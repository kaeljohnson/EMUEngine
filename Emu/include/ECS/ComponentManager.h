#pragma once

#include "../Includes.h"
#include "../Logging/Logger.h"

namespace Engine
{
    using Entity = size_t;

    class ComponentManagerBase 
    {
    public:
        virtual ~ComponentManagerBase() = default;
        virtual void Allocate(size_t size) = 0;
		virtual void ActivateComponents(std::vector<Entity>& entities) = 0;
        virtual void ActivateComponent(Entity entity) = 0;
		virtual void DeactivateComponents() = 0;
		virtual void DeactivateComponent(Entity entity) = 0;
		virtual void DestroyComponent(Entity entity) = 0;
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
		void ActivateComponents(std::vector<Entity>& entities) override
        {
            for (Entity entity : entities)
            {
				// size_t entity = entity->GetID();
                auto it = m_entityToIndex.find(entity);
                if (it != m_entityToIndex.end())
                {
                    size_t index = it->second;

                    // Move component to m_hotComponents
                    m_hotComponents.push_back(std::move(m_components[index]));
                    m_hotEntities.push_back(entity);
					m_hotIndices[entity] = m_hotComponents.size() - 1;

                    // If index is not the last element, move the last element to fill its place.
                    if (index != m_components.size() - 1)
                    {
                        m_components[index] = std::move(m_components.back());

                        // Update the index of the moved component in entityToIndex map.
                        size_t movedId = m_components[index].m_entity;
                        m_entityToIndex[movedId] = index;
                    }

                    m_components.pop_back();
                    m_entityToIndex.erase(it);
                }
            }
        }

        // Should just iterate through sparse set here to deactive components.
		void DeactivateComponents() override
        {
            for (T& component : m_hotComponents)
            {
                size_t entity = component.m_entity;

				// Move the component back to m_components in any order.
                m_components.push_back(std::move(component));
                m_entityToIndex[entity] = m_components.size() - 1;
            }

            m_hotComponents.clear();
			m_hotEntities.clear();
			m_hotIndices.fill(INVALID_INDEX);
            // m_hotIdToIndex.clear();
        }

        // Adds to or updates a component in the components array.
        template<typename... Args>
        void AddComponent(Args&&... args)
        {
            auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
            auto entity = std::get<0>(argsTuple);

            // if (m_entityToIndex.count(entity) || m_hotIdToIndex.count(entity))
			if (m_entityToIndex.count(entity) || m_hotIndices[entity] != INVALID_INDEX)
			{
				return; // Component already exists 
			}

            m_components.emplace_back(std::forward<Args>(args)...);
            m_entityToIndex[entity] = m_components.size() - 1;
        }

		// Finds component by entity entity and destroys it.
        void DestroyComponent(Entity entity) override
        {
            // size_t entity = ptrEntity->GetID();

			if (m_hotIndices[entity] != INVALID_INDEX)
			{
				size_t index = m_hotIndices[entity];
				size_t lastIndex = m_hotComponents.size() - 1;

				if (index != lastIndex)
				{
					std::swap(m_hotComponents[index], m_hotComponents[lastIndex]);
					m_hotEntities[index] = m_hotEntities[lastIndex]; // Swap the entity entity with the last one. No need to change last entity, as it will be popped.
					m_hotIndices[m_hotEntities[index]] = index;
					
				}

				m_hotComponents.pop_back();
				m_hotEntities.pop_back();
				m_hotIndices[entity] = INVALID_INDEX;
                return;
			}
        
            auto it = m_entityToIndex.find(entity);
            if (it != m_entityToIndex.end())
            {
                size_t index = it->second;
                size_t lastIndex = m_components.size() - 1;

                if (index != lastIndex) // If it's not the last element, swap with the last one
                {
                    std::swap(m_components[index], m_components[lastIndex]);
                    m_entityToIndex[m_components[index].m_entity] = index; // Update swapped entity index
                }

                m_components.pop_back();
                m_entityToIndex.erase(it);
                return;
            }
        }

		// Returns a component by entity pointer.
        T* GetComponent(Entity entity)
        {
			size_t hotIndex = m_hotIndices[entity];

            if (hotIndex != INVALID_INDEX)
            {
                return &m_hotComponents[hotIndex];
            }

		
            auto it = m_entityToIndex.find(entity);
            if (it != m_entityToIndex.end())
            {
                return &m_components[it->second];
            }

            return nullptr;
		}

        // Check if entity has a component attached.
        bool HasComponent(Entity entity)
        {
            return m_hotIndices[entity] != INVALID_INDEX || m_entityToIndex.contains(entity);
        }

        void ActivateComponent(Entity entity) override
        {
            auto it = m_entityToIndex.find(entity);

            if (it == m_entityToIndex.end())
            {
                return; // Component doesn't exist in m_components
            }

            size_t index = it->second;

            // Move component to hot array
            m_hotComponents.push_back(std::move(m_components[index]));
			m_hotEntities.push_back(entity);
            m_hotIndices[entity] = m_hotComponents.size() - 1; // Update hot index mapping

            // If it's not the last component, swap with the last one to keep m_components dense
            size_t lastIndex = m_components.size() - 1;
            if (index != lastIndex)
            {
                std::swap(m_components[index], m_components[lastIndex]);
                m_entityToIndex[m_components[index].m_entity] = index; // Update the swapped index
            }

            m_components.pop_back();  // Remove last element after swap
            m_entityToIndex.erase(it);    // Remove from the entity map
        }

        void DeactivateComponent(Entity entity) override
        {
            // auto it = m_hotIdToIndex.find(entity);
            // if (it == m_hotIdToIndex.end())
			size_t index = m_hotIndices[entity];
			if (index == INVALID_INDEX)
            {
                return; // If the component is not in the hot array, there's nothing to deactivate
            }

            // Move the component back to the main array
            m_components.push_back(std::move(m_hotComponents[index]));

            // Update the index mappings for hot and main arrays
            m_entityToIndex[entity] = m_components.size() - 1;  // The component has been added to the main array
            // m_hotIdToIndex.erase(it);  // Remove from the hot entity map
			m_hotIndices[entity] = INVALID_INDEX;

            // Swap the component to the last position in the hot array and pop it
            size_t lastIndex = m_hotComponents.size() - 1;
            if (index != lastIndex)  // If the component isn't the last one
            {
                std::swap(m_hotComponents[index], m_hotComponents[lastIndex]);  // Swap the component with the last one
				m_hotEntities[index] = m_hotEntities[lastIndex];  // Swap the entity entity with the last one. No need to change last entity, as it will be popped.
                m_hotEntityToIndex[m_hotEntities[index]] = index;
            }

            // Pop the last component in the hot array, as it's now deactivated
            m_hotComponents.pop_back();
			m_hotEntities.pop_back();
        }

		// Allocate memory for components and hot components array.
        void Allocate(size_t size) override
		{
            ENGINE_CRITICAL_D("Allocating " + std::to_string(size));
			m_components.reserve(size);
			m_hotComponents.reserve(size);
			m_hotIndices.fill(INVALID_INDEX);
		}

    private:
        std::unordered_map<size_t, size_t> m_entityToIndex;          // Maps entity entity to index in m_components
        std::unordered_map<size_t, size_t> m_hotEntityToIndex;       // Maps entity entity to index in m_hotComponents

        std::vector<T> m_components;                             // Holds all components
        // std::vector<T> m_hotComponents;                       // Holds hot components

        // For hot components, use "sparse set" approach for efficiency instead of maps.                                 
		const int INVALID_INDEX = -1;                          // Invalid index for hot indices

        // Temp for now, client needs to set size.
		std::array<size_t, 10000> m_hotIndices;                // Maps entity entity to index in m_hotComponents. The "sparse set" array.
		
		std::vector<size_t> m_hotEntities;                    // Maps hot index to entity entity. The entity entity of each hot component in same order as hot component array.
		std::vector<T> m_hotComponents;                        // Hot components. The "dense set" array.


    };



}