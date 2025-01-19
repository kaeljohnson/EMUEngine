#pragma once

#include "../Core.h"
#include "../Includes.h"
#include "ComponentManager.h"
#include "Entity.h"

namespace Engine
{
	using EntityID = size_t;

    class ECS
    {
    public:
        static void Initialize(size_t maxID) 
        {
            m_maxID = maxID;
        }
        
        static Entity* CreateEntity() 
        {
            if (m_usedIDs.size() >= m_maxID + 1) 
            {
                throw std::runtime_error("Error: All IDs are taken.");
            }

            for (size_t id = 0; id <= m_maxID; ++id) 
            {
                if (m_usedIDs.find(id) == m_usedIDs.end()) 
                {
                    m_usedIDs.insert(id);
                    Entity* ptrEntity = new Entity(id);
                    return ptrEntity;
                }
            }

            // This point should never be reached due to the earlier check
            throw std::runtime_error("Error: Unable to generate a new ID.");
        }

        static void DestroyEntity(Entity* ptrEntity) 
        {
            // Ensure the entity is active
            if (m_usedIDs.find(ptrEntity->GetID()) != m_usedIDs.end()) 
            {
                // Recycle the entity ID
                releaseID(ptrEntity->GetID());

				for (auto& manager : m_componentManagers)
				{
					manager.second->DestroyComponent(ptrEntity);
				}
            }
        }

		template <typename T>
		static void RegisterComponentManager()
		{
            m_componentManagers[std::type_index(typeid(T))] = std::make_unique<ComponentManager<T>>();
			m_componentManagers[std::type_index(typeid(T))]->Allocate(m_maxID);
		}

		template<typename T>
        static ComponentManager<T>& GetComponentManager()
        {
            auto it = m_componentManagers.find(std::type_index(typeid(T)));
            if (it != m_componentManagers.end())
            {
                return *static_cast<ComponentManager<T>*>(it->second.get());
			}
			else
			{
				throw std::runtime_error("Error: Component Manager not found.");
			}
        }
        
		static void LoadEntities(std::vector<Entity*>& entities)
		{
			for (auto& manager : m_componentManagers)
			{
				manager.second->LoadComponents(entities);
			}
		}

        static void UnloadEntities()
        {
            for (auto& manager : m_componentManagers)
            {
                manager.second->UnloadComponents();
            }
        }

		static void ActivateEntities()
		{
			for (auto& manager : m_componentManagers)
			{
				manager.second->ActivateComponents();
			}
		}

		static void Activate(Entity* entity)
		{
			for (auto& manager : m_componentManagers)
			{
				manager.second->ActivateComponent(entity);
			}
		}

		static void Deactivate(Entity* entity)
		{
			for (auto& manager : m_componentManagers)
			{
				manager.second->DeactivateComponent(entity);
			}
		}

        static void DeactivateEntities()
		{
			for (auto& manager : m_componentManagers)
			{
				manager.second->DeactivateComponents();
			}
		}

		template <typename T>
        static void RemoveEntity(Entity* ptrEntity)
        {
            auto it = m_componentManagers.find(std::type_index(typeid(T)));
            if (it != m_componentManagers.end())
            {
                static_cast<ComponentManager<T>*>(it->second.get())->RemoveComponent(ptrEntity);
            }
        }

		static void Cleanup()
		{
			m_componentManagers.clear();
        }

    private:
        // These must be exposed through API so the client app shares the same objects.
        EMU_API static EntityID m_maxID;
        EMU_API static std::unordered_set<size_t> m_usedIDs;
        EMU_API static std::unordered_map<std::type_index, std::unique_ptr<ComponentManagerBase>> m_componentManagers;

        static void releaseID(size_t id)
        {
            m_usedIDs.erase(id);
        }
    };
}