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
		virtual void LoadComponents(std::vector<Entity*>& entities) = 0;
		virtual void UnloadComponents() = 0;
		virtual void ActivateComponents(std::vector<Entity*>& entities) = 0;
		virtual void ActivateComponent(Entity* ptrEntity) = 0;
		virtual void DeactivateComponents() = 0;
		virtual void DeactivateComponent(Entity* ptrEntity) = 0;
		virtual void RemoveComponent(Entity* ptrEntity) = 0;
    };

    template <typename T>
    class ComponentManager : public ComponentManagerBase 
    {
    public:
        using iterator = typename std::vector<T>::iterator;
        using const_iterator = typename std::vector<T>::const_iterator;

        // Iterator for hot components (begin to end)
        auto begin() { return m_hotComponents.begin(); }
        auto end() { return m_hotComponents.end(); }
        auto begin() const { return m_hotComponents.begin(); }
        auto end() const { return m_hotComponents.end(); }

        // Iterator for all components (from begin to end)
        auto all_components_begin() { return m_components.begin(); }
        auto all_components_end() { return m_components.end(); }
        auto all_components_begin() const { return m_components.begin(); }
        auto all_components_end() const { return m_components.end(); }

        // Loads components into hot array, ordering them by priority.
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
					m_hotComponents.back().SetActive(true);

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

        // Unloads components, moving them from hot array back to component array.
        void UnloadComponents()
        {
            for (T& component : m_hotComponents)
            {
                size_t id = component.GetEntity()->GetID();
				component.SetActive(false);

				// Move the component back to m_components in any order.
                m_components.push_back(std::move(component));
                m_inactiveIdToIndex[id] = m_components.size() - 1;
            }

            m_hotComponents.clear();
            m_hotIdToIndex.clear();
        }

        // Sort hot components by priority
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

        // Adds or updates an inactive component
        template<typename... Args>
        void AddComponent(Args&&... args)
        {
            auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
            auto id = std::get<0>(argsTuple)->GetID();

            if (m_inactiveIdToIndex.count(id) || m_hotIdToIndex.count(id))
            {
                return; // Component already exists 
            }

            m_components.emplace_back(std::forward<Args>(args)...);
            m_inactiveIdToIndex[id] = m_components.size() - 1;
        }

        void RemoveComponent(Entity* ptrEntity) override
        {
			size_t id = ptrEntity->GetID();
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
			// Only call this function if the component certainly 
            // exists as it throws an exception otherwise. Call
			// HasComponent() to check if the component exists.

			size_t id = entity->GetID();
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

			throw std::runtime_error("Component does not exist.");
		}

        bool HasComponent(Entity* ptrEntity)
        {
			size_t id = ptrEntity->GetID();
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

		void ActivateComponent(Entity* entity) override
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

        void ActivateComponents(std::vector<Entity*>& entities) override
        {
            for (Entity* entity : entities)
            {
                ActivateComponent(entity);
            }
        }

        // Deactivate component by removing it from m_hotComponents
        void DeactivateComponent(Entity* entity) override
        {
            auto id = entity->GetID();
            auto it = m_hotIdToIndex.find(id);
            if (it == m_hotIdToIndex.end())
            {
                return;
            }

            size_t index = it->second;

            m_hotComponents[index].SetActive(false);
        }

        void DeactivateComponents() override
        {
			for (auto& component : m_hotComponents)
            {
				component.SetActive(false);
            }
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