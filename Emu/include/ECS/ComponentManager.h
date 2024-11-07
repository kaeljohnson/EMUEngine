#pragma once

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>

#include "Entity.h"

namespace Engine
{
    class ComponentManagerBase 
    {
    public:
        virtual ~ComponentManagerBase() = default;
        virtual void Allocate(size_t size) = 0;
        virtual void RemoveComponent(size_t id) = 0;
		virtual void LoadComponents(std::vector<Entity*>& entities) = 0;
		virtual void UnloadComponents() = 0;
		virtual void ActivateComponents(std::vector<Entity*>& entities) = 0;
		virtual void DeactivateComponents() = 0;
    };

    template <typename T>
    class ComponentManager : public ComponentManagerBase 
    {
    public:
        using iterator = typename std::vector<T>::iterator;
        using const_iterator = typename std::vector<T>::const_iterator;

        // Iterator for all components (begin to end)
        auto begin() { return m_components.begin(); }
        auto end() { return m_components.end(); }
        auto begin() const { return m_components.begin(); }
        auto end() const { return m_components.end(); }

        // Iterator for active components (from begin to end)
        auto active_begin() { return m_hotComponents.begin(); }
        auto active_end() { return m_hotComponents.end(); }
        auto active_begin() const { return m_hotComponents.begin(); }
        auto active_end() const { return m_hotComponents.end(); }

        // Loads and activates components, ordering them by priority
        void LoadComponents(std::vector<Entity*>& entities)
        {
            for (Entity* entity : entities)
            {
				size_t id = entity->GetID();
                auto it = m_inactiveIdToIndex.find(id);
                if (it != m_inactiveIdToIndex.end())
                {
                    size_t index = it->second;

                    // Move component to m_hotComponents
                    m_hotComponents.push_back(std::move(m_components[index]));

                    m_hotIdToIndex[id] = m_hotComponents.size() - 1;

                    // If index is not the last element, move the last element to fill its place
                    if (index != m_components.size() - 1)
                    {
                        m_components[index] = std::move(m_components.back());
                        // Update the index of the moved component in m_inactiveIdToIndex
                        size_t movedId = m_components[index].GetEntity()->GetID();
                        m_inactiveIdToIndex[movedId] = index;
                    }

                    m_components.pop_back();
                    m_inactiveIdToIndex.erase(it);
                }
            }

            SortHotComponentsByPriority();
        }


        // Adds or updates an inactive component
        template<typename... Args>
        void AddComponent(Args&&... args)
        {
            auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
            auto id = std::get<0>(argsTuple).GetID();

            if (m_inactiveIdToIndex.count(id) || m_hotIdToIndex.count(id))
            {
                return; // Component already exists
            }

            m_components.emplace_back(std::forward<Args>(args)...);
            m_inactiveIdToIndex[id] = m_components.size() - 1;
        }

        void RemoveComponent(size_t id) override
        {
            auto it = m_inactiveIdToIndex.find(id);
            if (it != m_inactiveIdToIndex.end())
            {
                m_components.erase(m_components.begin() + it->second);
                m_inactiveIdToIndex.erase(it);
                return;
            }

            it = m_hotIdToIndex.find(id);
            if (it != m_hotIdToIndex.end())
            {
                m_hotComponents.erase(m_hotComponents.begin() + it->second);
                m_hotIdToIndex.erase(it);
                return;
            }
        }

        std::vector<T>& GetComponents()
        {
            return m_hotComponents;
        }

		T* GetComponent(Entity* entity)
		{
			return GetComponent(entity->GetID());
		}

        T* GetComponent(size_t id)
        {
            // Call HasComponent before calling GetComponent to check if the component exists
            auto it = m_hotIdToIndex.find(id);
            if (it != m_hotIdToIndex.end())
            {
                return &m_hotComponents[it->second];
            }

            it = m_inactiveIdToIndex.find(id);
            if (it != m_inactiveIdToIndex.end())
            {
                return &m_components[it->second];
            }

            return nullptr;
        }

        void HasComponent(size_t id)
        {
            auto it = m_hotIdToIndex.find(id);
            if (it != m_hotIdToIndex.end())
            {
                return true;
            }

            it = m_inactiveIdToIndex.find(id);
            if (it != m_inactiveIdToIndex.end())
            {
                return true;
            }

            return false;
        }

        void ActivateComponent(Entity* entity)
		{
			auto id = entity->GetID();
            auto it = m_hotIdToIndex.find(id);
            if (it == m_hotIdToIndex.end())
            {
                return;
            }

            size_t index = it->second;

			m_hotComponents[index].SetActive(true);
        }

        // Deactivate component by removing it from m_hotComponents
        void DeactivateComponent(size_t id)
        {
            auto it = m_hotIdToIndex.find(id);
            if (it == m_hotIdToIndex.end())
            {
                return;
            }

            size_t index = it->second;

			m_hotComponents[index].SetActive(false);
        }

        void ActivateComponents(std::vector<Entity*>& entities) override
        {
            for (Entity* entity : entities)
            {
                ActivateComponent(entity);
            }
        }

        void DeactivateComponents() override
        {
			for (auto& component : m_hotComponents)
            {
				component.SetActive(false);
            }
        }

        // Sort hot components by priority based on m_hotComponents
        void SortHotComponentsByPriority()
        {
            std::sort(m_hotComponents.begin(), m_hotComponents.end(),
                [this](const T& a, const T& b)
                {
                    return a.GetEntity()->GetPriority() > b.GetEntity()->GetPriority();
                });

            // Update the m_hotIdToIndex map to reflect the new order
            for (size_t i = 0; i < m_hotComponents.size(); ++i)
            {
                size_t id = m_hotComponents[i].GetEntity()->GetID();
                m_hotIdToIndex[id] = i;
            }
        }

        // Unloads components, moving them from m_hotComponents back to m_components
        void UnloadComponents()
        {
            for (T& component : m_hotComponents)
            {
                size_t id = component.GetEntity()->GetID();

                // Move the component back to m_components (order is not important here)
                m_components.push_back(std::move(component));
                m_inactiveIdToIndex[id] = m_components.size() - 1;
            }

            m_hotComponents.clear();
            m_hotIdToIndex.clear();
        }

        void Allocate(size_t size) override
		{
			m_components.reserve(size);
			m_hotComponents.reserve(size);
		}

    private:
        std::unordered_map<size_t, size_t> m_inactiveIdToIndex;  // Maps entity ID to index in m_components
        std::unordered_map<size_t, size_t> m_hotIdToIndex;       // Maps entity ID to index in m_hotComponents

        std::vector<T> m_components;               // Holds inactive components
        std::vector<T> m_hotComponents;            // Holds active components
    };



}