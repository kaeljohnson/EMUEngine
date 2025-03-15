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

        // Adds to, or updates a component in, the components array.
        template<typename... Args>
        void AddComponent(Args&&... args)
        {
            auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
            Entity entity = std::get<0>(argsTuple);

            if (m_components.count(entity) || m_hotIndices[entity] != INVALID_INDEX)
            {
                return; // Component already exists. 
            }

            // m_components[entity] = T(std::forward<Args>(args)...);

            T t(std::forward<Args>(args)...); // Construct the Transform object
            m_components.emplace(entity, std::move(t)); // Move it into the map

        }

        // Finds component by entity and destroy it.
        void DestroyComponent(Entity entity) override
        {
            // Check hot components first.
            if (m_hotIndices[entity] != INVALID_INDEX)
            {
                size_t index = m_hotIndices[entity];
                size_t lastIndex = m_hotComponents.size() - 1;

                if (index != lastIndex) // Only swap if necessary.
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

            auto it = m_components.find(entity);
            if (it != m_components.end())
            {
				m_components.erase(it);
            }
        }

        // Loads components into hot array, ordering them by priority.
		void ActivateComponents(std::vector<Entity>& entities) override
        {
            for (Entity entity : entities)
            {
				ActivateComponent(entity);
            }
        }

        void ActivateComponent(Entity entity) override
        {
            auto it = m_components.find(entity);

            if (it == m_components.end())
            {
                return; // Component doesn't exist in m_components.
            }

            // Move component to hot array.
            m_hotComponents.push_back(std::move(it->second));
            m_hotEntities.push_back(entity);
            m_hotIndices[entity] = m_hotComponents.size() - 1; // Update hot index mapping


            m_components.erase(it);    // Remove from the cold map.
        }

		void DeactivateComponents() override
        {
            for (T& component : m_hotComponents)
            {
                m_components.emplace(component.m_entity, std::move(component));
            }

            m_hotComponents.clear();
			m_hotEntities.clear();
			m_hotIndices.fill(INVALID_INDEX);
        }

        void DeactivateComponent(Entity entity) override
        {
            size_t index = m_hotIndices[entity];
            if (index == INVALID_INDEX)
            {
                return; // If the component is not in the hot array, there's nothing to deactivate.
            }

            // Move the component back to the cold array.
			T& component = m_hotComponents[index];
            m_components.emplace(component.m_entity, std::move(component));

            // Update the index mappings for hot array.
            m_hotIndices[entity] = INVALID_INDEX;

            // Swap the component to the last position in the hot array and pop it.
			size_t lastIndex = m_hotComponents.size() - 1;
            if (index != lastIndex)
            {
                std::swap(m_hotComponents[index], m_hotComponents[lastIndex]);
                m_hotEntities[index] = m_hotEntities[lastIndex];  // Swap the entity entity with the last one. No need to change last entity, as it will be popped.
            }

            // Pop the last component in the hot array, as it's now deactivated
            m_hotComponents.pop_back();
            m_hotEntities.pop_back();
        }

		// Returns a component by entity pointer.
        T* GetComponent(Entity entity)
        {
			size_t hotIndex = m_hotIndices[entity];

            if (hotIndex != INVALID_INDEX)
            {
                return &m_hotComponents[hotIndex];
            }
		
            auto it = m_components.find(entity);
            if (it != m_components.end())
            {
                return &it->second;
            }

            return nullptr;
		}

        // Check if entity has a component attached.
        bool HasComponent(Entity entity)
        {
            return m_hotIndices[entity] != INVALID_INDEX || m_components.contains(entity);
        }

		// Allocate memory for components and hot components array.
        void Allocate(size_t size) override
		{
            ENGINE_CRITICAL_D("Allocating " + std::to_string(size));
			m_hotComponents.reserve(size);
			m_hotIndices.fill(INVALID_INDEX);
		}

    private:
		std::unordered_map<size_t, T> m_components;             // Holds all components

		const int INVALID_INDEX = -1;                           // Invalid index for hot indices

        // Temp for now, client needs to set size.
		std::array<size_t, 10000> m_hotIndices;                 // Maps entity entity to index in m_hotComponents. The "sparse set" array.
		
		std::vector<size_t> m_hotEntities;                      // Maps hot index to entity entity. The entity entity of each hot component in same order as hot component array.
		std::vector<T> m_hotComponents;                         // Hot components. The "dense set" array.


    };



}