#pragma once

#include "../Includes.h"
#include "../Logging/Logger.h"

namespace Engine
{
    using Entity = size_t;

    /**
	* @brief Base class for component managers.
	*
	* Manages the lifecycle and storage of components of a specific type.
	*/
    
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
		virtual const size_t GetNumActiveComponents() const = 0;
		virtual bool HasComponent(Entity entity) = 0;
		virtual bool IsActive(Entity entity) = 0;
    };


    /**
	* @brief Templated component manager for managing components of type T.
    * 
	* Provides functions to add, remove, activate, and deactivate components.
    */
    template <typename T>
    class ComponentManager : public ComponentManagerBase 
    {
    public:

        /**
		* @brief Adds a new component of type T for the specified entity.
        * 
		* * @tparam Args Variadic template parameters for component constructor.
        */
        template<typename... Args>
        void AddComponent(Args&&... args)
        {
            auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
            Entity entity = (Entity)std::get<0>(argsTuple);

            if (m_components.count(entity) || m_hotIndices[entity] != INVALID_INDEX)
            {
				throw std::runtime_error("Error: Component already exists.");
            }

            try
            {
                m_components.emplace(entity, T(std::forward<Args>(args)...)); // Move it into the map
            } 
            catch (const std::exception& e)
            {
                std::cerr << "Failed to add component: " << std::string(e.what());
            }
        }

        /**
		* @brief Destroys the component associated with the specified entity.
		* Destroying a component erases it from both hot and cold storage.
		* This is different from deactivating a component, which only moves it from hot to cold storage.
        * 
		* * @param entity The entity whose component is to be destroyed.
        */
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

        /**
		* @brief Activates components for the specified entities.
		* Calls the ActivateComponent function for each entity in the vector. 
        * 
		* * @param The entities whose components are to be activated.
        */
		void ActivateComponents(std::vector<Entity>& entities) override
        {
            for (Entity entity : entities)
            {
				ActivateComponent(entity);
            }
        }

        /**
		* @brief Activates the component associated with the specified entity.
		* Activation moves the component from cold storage (unordered_map) to
		* hot storage (contiguous vector) for better cache performance at runtime.
        * 
		* * @param The entity whose component is to be activated.
        */
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

		/**
		* @brief Deactivates all active components.
		* Deactivation moves all components from hot storage (contiguous vector)
		* back to cold storage (unordered_map).
		*/
		void DeactivateComponents() override
        {
            for (T& component : m_hotComponents)
            {
                m_components.emplace(component.m_entity, std::move(component));
            }

            m_hotComponents.clear();
			m_hotEntities.clear();
			m_hotIndices.assign(m_maxComponents, INVALID_INDEX);
        }

		/**
		* @brief Deactivates the component associated with the specified entity.
		* Deactivation moves the component from hot storage (contiguous vector)
		* back to cold storage (unordered_map).
        * 
		* * @param The entity whose component is to be deactivated.
		*/
        void DeactivateComponent(Entity entity) override
        {
			if (entity > m_maxComponents || entity < 0)
			{
				ENGINE_ERROR_D("Trying to deactivate component for invalid entity (entity does not exist or was created improperly): " + std::to_string(entity));
				return; // Invalid entity, nothing to deactivate.
			}

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
				m_hotIndices[m_hotEntities[index]] = index;
            }

            // Pop the last component in the hot array, as it's now deactivated
            m_hotComponents.pop_back();
            m_hotEntities.pop_back();
        }

		/**
		* @brief Retrieves a pointer to the component associated with the specified entity.
		* Checks the hot components first, then the cold components.
        * 
		* * @param The entity whose component is to be retrieved.
		* * @return Pointer to the component if found, nullptr otherwise.
        */
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

        /**
		* @brief Checks if a component exists for the specified entity.
        * 
		* * @param The entity to check for a component.
		* * @return True if the component exists, false otherwise.
        */
        bool HasComponent(Entity entity)
        {
            return m_hotIndices[entity] != INVALID_INDEX || m_components.contains(entity);
        }

		/**
		* @brief Checks if the component associated with the specified entity is active.
		* An active component is one that is currently in hot storage.
		*
		* * @param The entity to check for an active component.
		* * @return True if the component is active, false otherwise.
		*/
		bool IsActive(Entity entity) override
		{
			return m_hotIndices[entity] != INVALID_INDEX;
		}

		/**
		* @brief Allocates memory for the components and hot components array.
		*
		* * @param maxComponents The maximum number of components to allocate memory for.
		*/
        void Allocate(size_t maxComponents) override
		{
			m_hotComponents.reserve(maxComponents);
			m_hotEntities.reserve(maxComponents);
            m_hotIndices.assign(maxComponents, INVALID_INDEX);

			m_maxComponents = maxComponents;
		}

		/**
		* @brief Gets the number of active components.
		*
		* * @return The number of active components.
		*/
		const size_t GetNumActiveComponents() const override { return m_hotComponents.size(); }

		/**
		* @brief Gets the vector of hot components.
        * 
		* @return Reference to the vector of hot components.
		*/
		std::vector<T>& GetHotComponents() { return m_hotComponents; }

    private:
		std::unordered_map<size_t, T> m_components;             /// Holds all components

		const int INVALID_INDEX = -1;                           /// Invalid index for hot indices
		size_t m_maxComponents = 0;

		std::vector<size_t> m_hotIndices;                       /// Maps entity entity to index in m_hotComponents. The "sparse set" array.
		std::vector<size_t> m_hotEntities;                      /// Maps hot index to entity entity. The entity entity of each hot component in same order as hot component array.
		std::vector<T> m_hotComponents;                         /// Hot components. The "dense set" array.
    };



}