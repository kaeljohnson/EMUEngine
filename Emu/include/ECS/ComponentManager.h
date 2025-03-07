#pragma once

#include "../Includes.h"
#include "Entity.h"
#include "../Logging/Logger.h"

namespace Engine
{
    class ComponentManagerBase 
    {
    public:
        virtual ~ComponentManagerBase() = default;
        virtual void Allocate(size_t size) = 0;
		virtual void ActivateComponents(std::vector<Entity*>& entities) = 0;
        virtual void ActivateComponent(Entity* ptrEntity) = 0;
		virtual void DeactivateComponents() = 0;
		virtual void DeactivateComponent(Entity* ptrEntity) = 0;
		virtual void DestroyComponent(Entity* ptrEntity) = 0;
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
        void ActivateComponents(std::vector<Entity*>& entities)
        {
            for (Entity* entity : entities)
            {
				size_t id = entity->GetID();
                auto it = m_idToIndex.find(id);
                if (it != m_idToIndex.end())
                {
                    size_t index = it->second;

                    // Move component to m_hotComponents
                    m_hotComponents.push_back(std::move(m_components[index]));

                    m_hotIdToIndex[id] = m_hotComponents.size() - 1;

                    // If index is not the last element, move the last element to fill its place.
                    if (index != m_components.size() - 1)
                    {
                        m_components[index] = std::move(m_components.back());

                        // Update the index of the moved component in idToIndex map.
                        size_t movedId = m_components[index].GetEntity()->GetID();
                        m_idToIndex[movedId] = index;
                    }

                    m_components.pop_back();
                    m_idToIndex.erase(it);
                }
            }
        }

        void DeactivateComponents()
        {
            for (T& component : m_hotComponents)
            {
                size_t id = component.GetEntity()->GetID();

				// Move the component back to m_components in any order.
                m_components.push_back(std::move(component));
                m_idToIndex[id] = m_components.size() - 1;
            }

            m_hotComponents.clear();
            m_hotIdToIndex.clear();
        }

        // Adds to or updates a component in the components array.
        template<typename... Args>
        void AddComponent(Args&&... args)
        {
            auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
            auto id = std::get<0>(argsTuple)->GetID();

            if (m_idToIndex.count(id) || m_hotIdToIndex.count(id))
            {
                return; // Component already exists 
            }

            m_components.emplace_back(std::forward<Args>(args)...);
            m_idToIndex[id] = m_components.size() - 1;
        }

		// Finds component by entity ID and destroys it.
        void DestroyComponent(Entity* ptrEntity) override
        {
            size_t id = ptrEntity->GetID();

            auto it = m_hotIdToIndex.find(id);
            if (it != m_hotIdToIndex.end())
            {
                size_t index = it->second;
                size_t lastIndex = m_hotComponents.size() - 1;

                if (index != lastIndex)
                {
                    std::swap(m_hotComponents[index], m_hotComponents[lastIndex]);
                    m_hotIdToIndex[m_hotComponents[index].GetEntity()->GetID()] = index;
                }

                m_hotComponents.pop_back();
                m_hotIdToIndex.erase(it);
                return;
            }

            it = m_idToIndex.find(id);
            if (it != m_idToIndex.end())
            {
                size_t index = it->second;
                size_t lastIndex = m_components.size() - 1;

                if (index != lastIndex) // If it's not the last element, swap with the last one
                {
                    std::swap(m_components[index], m_components[lastIndex]);
                    m_idToIndex[m_components[index].GetEntity()->GetID()] = index; // Update swapped entity index
                }

                m_components.pop_back();
                m_idToIndex.erase(it);
                return;
            }
        }


		// Returns a component by entity pointer. Throw exception if component does not exist.
		T* GetComponent(Entity* ptrEntity)
		{
			// Only call this function if the component certainly 
            // exists as it throws an exception otherwise. Call
			// HasComponent() to check if the component exists.

			size_t id = ptrEntity->GetID();
            auto it = m_hotIdToIndex.find(id);
            if (it != m_hotIdToIndex.end())
            {
                return &m_hotComponents[it->second];
            }

            it = m_idToIndex.find(id);
            if (it != m_idToIndex.end())
            {
                return &m_components[it->second];
            }

            return nullptr;
		}

        // Check if entity has a component attached.
        bool HasComponent(Entity* ptrEntity)
        {
            size_t id = ptrEntity->GetID();
            return m_hotIdToIndex.contains(id) || m_idToIndex.contains(id);
        }

        void ActivateComponent(Entity* ptrEntity) override
        {
            size_t id = ptrEntity->GetID();
            auto it = m_idToIndex.find(id);

            if (it == m_idToIndex.end())
            {
                return; // Component doesn't exist in m_components
            }

            size_t index = it->second;

            // Move component to hot array
            m_hotComponents.push_back(std::move(m_components[index]));
            m_hotIdToIndex[id] = m_hotComponents.size() - 1; // Update hot index mapping

            // If it's not the last component, swap with the last one to keep m_components dense
            size_t lastIndex = m_components.size() - 1;
            if (index != lastIndex)
            {
                std::swap(m_components[index], m_components[lastIndex]);
                m_idToIndex[m_components[index].GetEntity()->GetID()] = index; // Update the swapped index
            }

            m_components.pop_back();  // Remove last element after swap
            m_idToIndex.erase(it);    // Remove from the id map
        }

        void DeactivateComponent(Entity* entity) override
        {
            auto id = entity->GetID();
            auto it = m_hotIdToIndex.find(id);
            if (it == m_hotIdToIndex.end())
            {
                return; // If the component is not in the hot array, there's nothing to deactivate
            }

            size_t index = it->second;

            // Move the component back to the main array
            m_components.push_back(std::move(m_hotComponents[index]));

            // Update the index mappings for hot and main arrays
            m_idToIndex[id] = m_components.size() - 1;  // The component has been added to the main array
            m_hotIdToIndex.erase(it);  // Remove from the hot id map

            // Swap the component to the last position in the hot array and pop it
            size_t lastIndex = m_hotComponents.size() - 1;
            if (index != lastIndex)  // If the component isn't the last one
            {
                std::swap(m_hotComponents[index], m_hotComponents[lastIndex]);  // Swap the component with the last one
                m_hotIdToIndex[m_hotComponents[index].GetEntity()->GetID()] = index;
            }

            // Pop the last component in the hot array, as it's now deactivated
            m_hotComponents.pop_back();
        }

		// Allocate memory for components and hot components array.
        void Allocate(size_t size) override
		{
            ENGINE_CRITICAL_D("Allocating " + std::to_string(size));
			m_components.reserve(size);
			m_hotComponents.reserve(size);
		}

    private:
        std::unordered_map<size_t, size_t> m_idToIndex;          // Maps entity ID to index in m_components
        std::unordered_map<size_t, size_t> m_hotIdToIndex;       // Maps entity ID to index in m_hotComponents

        std::vector<T> m_components;                             // Holds all components
        std::vector<T> m_hotComponents;                          // Holds hot components
    };



}