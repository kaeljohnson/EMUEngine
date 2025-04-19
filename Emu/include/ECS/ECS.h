#pragma once

#include "../Includes.h"
#include "ComponentManager.h"

namespace Engine
{
	using Entity = size_t;

    class ECS
    {
    public:
		Entity INVALID_ENTITY = -1;

        void Initialize(size_t numEntities) 
        {
			m_numEntities = numEntities;
			INVALID_ENTITY = numEntities + 1;
        }
        
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

		// Destroy all components associated with the entity.
		void DestroyComponents(Entity entity)
		{
			if (m_usedIDs.find(entity) == m_usedIDs.end())
				throw std::runtime_error("Error: Entity does not exist.");

			for (auto& manager : m_componentManagers)
			{
				manager.second->DestroyComponent(entity);
			}
		}

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

		bool HasEntity(Entity entity)
		{
			return m_usedIDs.find(entity) != m_usedIDs.end();
		}

		template <typename T>
		bool IsActive(Entity entity)
		{
			return static_cast<ComponentManager<T>*>(m_componentManagers[std::type_index(typeid(T))].get())->IsActive(entity);
		}

		template <typename T>
		void RegisterComponentManager()
		{
            m_componentManagers[std::type_index(typeid(T))] = std::make_unique<ComponentManager<T>>();
			m_componentManagers[std::type_index(typeid(T))]->Allocate(m_numEntities);
		}

        template <typename T>
		bool HasComponentManager()
		{
			return m_componentManagers.find(std::type_index(typeid(T))) != m_componentManagers.end();
		}

		template <typename T, typename... Args>
		void AddComponent(Entity entity, Args&&... componentArgs)
		{
			if (m_usedIDs.find(entity) == m_usedIDs.end())
				throw std::runtime_error("Error: Entity does not exist.");

			static_cast<ComponentManager<T>*>(m_componentManagers[std::type_index(typeid(T))].get())->AddComponent(entity, std::forward<Args>(componentArgs)...);
		}

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

		template <typename T>
		void DestroyComponent(Entity entity)
		{
			m_componentManagers[std::type_index(typeid(T))]->DestroyComponent(entity);
		}

        template <typename T>
		void DestroyComponentSafe(Entity entity)
		{
			auto it = m_componentManagers.find(std::type_index(typeid(T)));
			if (it != m_componentManagers.end())
			{
				m_componentManagers[it]->DestroyComponent(entity);
			}
		}

		template <typename T>
		T* GetComponent(Entity entity)
		{
			return static_cast<ComponentManager<T>*>(m_componentManagers[std::type_index(typeid(T))].get())->GetComponent(entity);
		}

		template <typename T>
		std::vector<T>& GetComponentSafe(Entity entity)
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

		template<typename T>
		std::vector<T>& GetHotComponents()
		{
			return static_cast<ComponentManager<T>*>(m_componentManagers[std::type_index(typeid(T))].get())->GetHotComponents();
		}

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

		template<typename T>
		size_t GetNumActiveComponents()
		{
			return m_componentManagers[std::type_index(typeid(T))]->GetNumActiveComponents();
		}

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
        
		void ActivateEntities(std::vector<Entity>& entities)
		{
			for (auto& manager : m_componentManagers)
			{
				manager.second->ActivateComponents(entities);
			}
		}

		void Activate(Entity entity)
		{
			for (auto& manager : m_componentManagers)
			{
				manager.second->ActivateComponent(entity);
			}
		}

		template<typename T>
		void ActivateComponent(Entity entity)
		{
			m_componentManagers[std::type_index(typeid(T))]->ActivateComponent(entity);
		}

		template<typename T>
		void Activate(Entity entity)
		{
			return m_componentManagers[std::type_index(typeid(T))]->ActivateComponent(entity);
		}

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

        void DeactivateEntities()
        {
            for (auto& manager : m_componentManagers)
            {
                manager.second->DeactivateComponents();
            }
        }

		void Deactivate(Entity entity)
		{
			for (auto& manager : m_componentManagers)
			{
				manager.second->DeactivateComponent(entity);
			}
		}

		template<typename T>
		void Deactivate(Entity entity)
		{
			return m_componentManagers[std::type_index(typeid(T))]->DeactivateComponent(entity);
		}

		template<typename T>
		void DeactivateComponent(Entity entity)
		{
			m_componentManagers[std::type_index(typeid(T))]->DeactivateComponent(entity);
		}

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

		size_t GetNumEntities() const
		{
			return m_usedIDs.size();
		}

		void Cleanup()
		{
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