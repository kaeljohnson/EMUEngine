#pragma once

#include "../Includes.h"
#include "ComponentManager.h"

namespace Engine
{
	/**
	* @brief Type alias for Entity, represented as a size_t.
	*/
	using Entity = size_t;

	/**
	* @brief Entity Component System (ECS) class for managing entities and their components.
	* Provides methods to create, destroy, and manage entities and their associated components.
	*/
    class ECS
    {
    public:
		Entity INVALID_ENTITY = -1;

		/**
		* @brief Initializes the ECS with a specified number of entities.
		*
		* * @param numEntities: The maximum number of entities that can be created.
		*/
        void Initialize(size_t numEntities) 
        {
			m_numEntities = numEntities;
			INVALID_ENTITY = numEntities + 1;
        }

		/**
		* @brief Creates a new entity and returns its ID.
		* 
		* * @return The ID of the newly created entity.
		*/
        Entity CreateEntity() 
        {
            if (m_usedIDs.size() >= m_numEntities) 
            {
                throw std::runtime_error("Error: All IDs are taken.");
            }

            for (size_t id = 0; id < m_numEntities; ++id) 
            {
                if (m_usedIDs.find(id) == m_usedIDs.end()) 
                {
                    m_usedIDs.insert(id);
                    return id;
                }
            }

			return -1;
        }

		/**
		* @brief Destroys the specified entity and all its associated components.
		* Destroying an entity removes it from the ECS and frees up its ID for future use.
		* When an entity is destroyed, all components associated with that entity are also destroyed.
		* 
		* * @param entity: The ID of the entity to be destroyed.
		*/
		void DestroyEntity(Entity entity)
		{
			if (m_usedIDs.find(entity) == m_usedIDs.end())
				throw std::runtime_error("Error: Entity does not exist.");

			m_usedIDs.erase(entity);
			for (auto& manager : m_componentManagers)
			{
				manager.second->DestroyComponent(entity);
			}
		}

		/** 
		* @brief Destroy all components associated with the entity.
		* Call Destroy on each component manager to remove components linked to the specified entity.
		* 
		* * @param entity: The entity whose components are to be destroyed.
		*/
		void DestroyComponents(Entity entity)
		{
			if (m_usedIDs.find(entity) == m_usedIDs.end())
				throw std::runtime_error("Error: Entity does not exist.");

			for (auto& manager : m_componentManagers)
			{
				manager.second->DestroyComponent(entity);
			}
		}

		/**
		* @brief Destroy all components associated with the entities.
		* Call Destroy on each component manager to remove components linked to the specified entities.
		* 
		* * @param entities: The entities whose components are to be destroyed.
		*/
		void DestroyComponents(std::vector<Entity>& entities)
		{
			for (auto& entity : entities)
			{
				if (entity >= m_numEntities) // Entity is out of range.
					throw std::runtime_error("Error: Entity does not exist: " + std::to_string(entity));

				auto it = m_usedIDs.find(entity);
				if (it != m_usedIDs.end())
				{
					for (auto& manager : m_componentManagers)
					{
						manager.second->DestroyComponent(entity);
					}
				}
				else
				{
					throw std::runtime_error("Error: Entity does not exist: " + std::to_string(entity));
				}
			}
		}

		/**
		* @brief Checks if the specified entity exists.
		* 
		* * @param entity: The ID of the entity to check.
		* * @return True if the entity exists, false otherwise.
		*/
		bool HasEntity(Entity entity)
		{
			return m_usedIDs.find(entity) != m_usedIDs.end();
		}

		/**
		* @brief Checks if the component associated with the specified entity is active.
		* 
		* * @tparam T: The type of the component to check.
		* * @param entity: The entity to check for an active component.
		* * @return True if the component is active, false otherwise.
		*/
		template <typename T>
		bool IsActive(Entity entity)
		{
			return static_cast<ComponentManager<T>*>(m_componentManagers[std::type_index(typeid(T))].get())->IsActive(entity);
		}

		/**
		* @brief Registers a component manager for the specified component type.
		* 
		* * @tparam T: The type of the component.
		*/
		template <typename T>
		void RegisterComponentManager()
		{
            m_componentManagers[std::type_index(typeid(T))] = std::make_unique<ComponentManager<T>>();
			m_componentManagers[std::type_index(typeid(T))]->Allocate(m_numEntities);
		}

		/**
		* @brief Checks if a component manager for the specified component type exists.
		* 
		* * @tparam T: The type of the component.
		*/
        template <typename T>
		bool HasComponentManager()
		{
			return m_componentManagers.find(std::type_index(typeid(T))) != m_componentManagers.end();
		}

		/**
		* @brief Adds a component of type T to the specified entity.
		* 
		* * @tparam T: The type of the component to add.
		* * @param entity: The entity to which the component will be added.
		* * @param componentArgs: The arguments to construct the component.
		*/
		template <typename T, typename... Args>
		void AddComponent(Entity entity, Args&&... componentArgs)
		{
			if (m_usedIDs.find(entity) == m_usedIDs.end())
				throw std::runtime_error("Error: Entity does not exist.");

			static_cast<ComponentManager<T>*>(m_componentManagers[std::type_index(typeid(T))].get())->AddComponent(entity, std::forward<Args>(componentArgs)...);
		}

		/**
		* @brief Safely adds a component of type T to the specified entity.
		* Checks if the component manager exists before adding the component.
		*
		* * @tparam T: The type of the component to add.
		* * @param entity: The entity to which the component will be added.
		* * @param componentArgs: The arguments to construct the component.
		*/
        template <typename T, typename... Args>
        void AddComponentSafe(Entity entity, Args&&... componentArgs)
        {
			if (m_usedIDs.find(entity) == m_usedIDs.end())
				throw std::runtime_error("Error: Entity does not exist.");

            auto it = m_componentManagers.find(std::type_index(typeid(T)));
            if (it != m_componentManagers.end())
            {
                static_cast<ComponentManager<T>*>(it->second.get())->AddComponent(entity, std::forward<Args>(componentArgs)...);
            }
			else
			{
				throw std::runtime_error("Error: Component Manager not found.");
			}
        }

		/**
		* @brief Destroys the component of type T associated with the specified entity.
		* Call DestroyComponent on the appropriate component manager to remove the component.
		* 
		* * @tparam T: The type of the component to destroy.
		* * @param entity: The entity whose component is to be destroyed.
		*/
		template <typename T>
		void DestroyComponent(Entity entity)
		{
			m_componentManagers[std::type_index(typeid(T))]->DestroyComponent(entity);
		}

		/**
		* @brief Safely destroys the component of type T associated with the specified entity.
		* Checks if the component manager exists before attempting to destroy the component.
		* Call DestroyComponent on the appropriate component manager to remove the component.
		* 
		* * @tparam T: The type of the component to destroy.
		* * @param entity: The entity whose component is to be destroyed.
		*/
        template <typename T>
		void DestroyComponentSafe(Entity entity)
		{
			auto it = m_componentManagers.find(std::type_index(typeid(T)));
			if (it != m_componentManagers.end())
			{
				m_componentManagers[it]->DestroyComponent(entity);
			}
		}

		/**
		* @brief Retrieves the component of type T associated with the specified entity.
		* 
		* * @tparam T: The type of the component to retrieve.
		* * @param entity: The entity whose component is to be retrieved.
		* * @return A pointer to the component of type T associated with the entity.
		*/
		template <typename T>
		T* GetComponent(Entity entity)
		{
			return static_cast<ComponentManager<T>*>(m_componentManagers[std::type_index(typeid(T))].get())->GetComponent(entity);
		}

		/**
		* @brief Safely retrieves the component of type T associated with the specified entity.
		* Checks if the component manager exists before attempting to retrieve the component.
		* 
		* * @tparam T: The type of the component to retrieve.
		* * @param entity: The entity whose component is to be retrieved.
		* * @return A pointer to the component of type T associated with the entity.
		*/
		template <typename T>
		T* GetComponentSafe(Entity entity)
		{
			auto it = m_componentManagers.find(std::type_index(typeid(T)));
			if (it != m_componentManagers.end())
			{
				return static_cast<ComponentManager<T>*>(it->second.get())->GetComponent(entity);
			}
			else
			{
				throw std::runtime_error("Error: Component Manager not found.");
			}
		}

		/**
		* @brief Checks if the specified entity has a component of type T.
		* 
		* * @tparam T: The type of the component to check.
		* * @param entity: The entity to check for the component.
		* * @return True if the entity has the component, false otherwise.
		*/
		template<typename T>
		bool HasComponent(Entity entity)
		{
			if (entity >= m_numEntities || entity < 0) // Component manager "HasComponent" will check an invalid index if entity too big.
			{
				ENGINE_ERROR_D("Entity does not exist. Cannot check for has component. Returning false: " + std::to_string(entity));
				return false;
			}

			return m_componentManagers[std::type_index(typeid(T))]->HasComponent(entity);
		}

		/**
		* @brief Safely checks if the specified entity has a component of type T.
		* Checks if the component manager exists before attempting to check for the component.
		* 
		* * @tparam T: The type of the component to check.
		* * @param entity: The entity to check for the component.
		* * @return True if the entity has the component, false otherwise.
		*/
        template<typename T>
		bool HasComponentSafe(Entity entity)
		{
			if (entity >= m_numEntities || entity < 0) // Component manager "HasComponent" will check an invalid index if entity too big.
				throw std::runtime_error("Error: Entity does not exist.");

			auto it = m_componentManagers.find(std::type_index(typeid(T)));
			if (it != m_componentManagers.end())
			{
				return m_componentManagers[it]->HasComponent(entity);
			}
			return false;
		}

		/**
		* @brief Retrieves a reference to the vector of active components of type T.
		*
		* * @tparam T: The type of the components to retrieve.
		* * @return Ref to the hot components vector of type T. 
		*/
		template<typename T>
		std::vector<T>& GetHotComponents()
		{
			return static_cast<ComponentManager<T>*>(m_componentManagers[std::type_index(typeid(T))].get())->GetHotComponents();
		}

		/**
		* @brief Safely retrieves a reference to the vector of active components of type T.
		* Checks if the component manager exists before attempting to retrieve the components.
		* 
		* * @tparam T: The type of the components to retrieve.
		* * @return Ref to the hot components vector of type T.
		*/ 
		template<typename T>
		std::vector<T>& GetHotComponentsSafe()
		{
			auto it = m_componentManagers.find(std::type_index(typeid(T)));
			if (it != m_componentManagers.end())
			{
				return static_cast<ComponentManager<T>*>(it->second.get())->GetHotComponents();
			} 
			else
			{
				throw std::runtime_error("Error: Component Manager not found.");
			}
		}

		/**
		* @brief Retrieves the number of active components of type T.
		* 
		* * @tparam T: The type of the components to count.
		* * @return The number of active components of type T.
		*/
		template<typename T>
		size_t GetNumActiveComponents()
		{
			return m_componentManagers[std::type_index(typeid(T))]->GetNumActiveComponents();
		}

		/**
		* @brief Safely retrieves the number of active components of type T.
		* Checks if the component manager exists before attempting to count the components.
		* 
		* * @tparam T: The type of the components to count.
		* * @return The number of active components of type T.
		*/
		template<typename T>
		size_t GetNumActiveComponentsSafe()
		{
			auto it = m_componentManagers.find(std::type_index(typeid(T)));
			if (it != m_componentManagers.end())
			{
				return m_componentManagers[it]->GetNumActiveComponents();
			}
			else
			{
				throw std::runtime_error("Error: Component Manager not found.");
			}
		}
        
		/**
		* @brief Activates components for a list of entities.
		* 
		* * @param entities: The list of entities whose components are to be activated.
		*/
		void ActivateEntities(std::vector<Entity>& entities)
		{
			for (auto& manager : m_componentManagers)
			{
				manager.second->ActivateComponents(entities);
			}
		}

		/**
		* @brief Activates components for a specific entity.
		* 
		* @param entity: The entity whose components are to be activated.
		*/
		void Activate(Entity entity)
		{
			for (auto& manager : m_componentManagers)
			{
				manager.second->ActivateComponent(entity);
			}
		}

		/**
		* @brief Activates the component of type T associated with the specified entity.
		* 
		* * @tparam T: The type of the component to activate.
		* * @param entity: The entity whose component is to be activated.
		*/
		template<typename T>
		void ActivateComponent(Entity entity)
		{
			m_componentManagers[std::type_index(typeid(T))]->ActivateComponent(entity);
		}

		/**
		* @brief Activates the component of type T associated with the specified entity.
		* 
		* * @tparam T: The type of the component to activate.
		* * @param entity: The entity whose component is to be activated.
		*/
		template<typename T>
		void Activate(Entity entity)
		{
			return m_componentManagers[std::type_index(typeid(T))]->ActivateComponent(entity);
		}

		/**
		* @brief Safely activates the component of type T associated with the specified entity.
		* 
		* * @tparam T: The type of the component to activate.
		* * @param entity: The entity whose component is to be activated.
		*/
		template<typename T>
		void ActivateSafe(Entity entity)
		{
			auto it = m_componentManagers.find(std::type_index(typeid(T)));
			if (it != m_componentManagers.end())
			{
				m_componentManagers[it]->ActivateComponent(entity);
			}
			else
			{
				throw std::runtime_error("Error: Component Manager not found.");
			}
		}

		/**
		* @brief Deactivates components for all entities.
		*/
        void DeactivateEntities()
        {
            for (auto& manager : m_componentManagers)
            {
                manager.second->DeactivateComponents();
            }
        }

		/**
		* @brief Deactivates components for a specific entity.
		* 
		* @param entity: The entity whose components are to be deactivated.
		*/
		void Deactivate(Entity entity)
		{
			for (auto& manager : m_componentManagers)
			{
				manager.second->DeactivateComponent(entity);
			}
		}
		
		/**
		* @brief Deactivates the component of type T associated with the specified entity.
		* 
		* * @tparam T: The type of the component to deactivate.
		* * @param entity: The entity whose component is to be deactivated.
		*/
		template<typename T>
		void Deactivate(Entity entity)
		{
			return m_componentManagers[std::type_index(typeid(T))]->DeactivateComponent(entity);
		}

		/**
		* @brief Deactivates the component of type T associated with the specified entity.
		* 
		* * @tparam T: The type of the component to deactivate.
		* * @param entity: The entity whose component is to be deactivated.
		*/
		template<typename T>
		void DeactivateComponent(Entity entity)
		{
			m_componentManagers[std::type_index(typeid(T))]->DeactivateComponent(entity);
		}

		/**
		* @brief Safely deactivates the component of type T associated with the specified entity.
		* Finds the component manager for type T and deactivates the component for the given entity.
		* 
		* * @tparam T: The type of the component to deactivate.
		* * @param entity: The entity whose component is to be deactivated.
		*/
		template<typename T>
		void DeactivateSafe(Entity entity)
		{
			auto it = m_componentManagers.find(std::type_index(typeid(T)));
			if (it != m_componentManagers.end())
			{
				m_componentManagers[it]->DeactivateComponent(entity);
			}
			else
			{
				throw std::runtime_error("Error: Component Manager not found.");
			}
		}

		/**
		* @brief Retrieves the number of currently active entities.
		* 
		* * @return The number of active entities.
		*/
		size_t GetNumEntities() const
		{
			return m_usedIDs.size();
		}

		/**
		* @brief Cleans up the ECS by clearing all component managers.
		*/
		void Cleanup()
		{
			// Needs to do any other cleanup first?
			m_componentManagers.clear();
        }

    private:
        Entity m_numEntities = 0;
        std::unordered_set<size_t> m_usedIDs;
        std::unordered_map<std::type_index, std::unique_ptr<ComponentManagerBase>> m_componentManagers;

        void releaseID(size_t id)
        {
            m_usedIDs.erase(id);
        }
    };
}