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
        void Initialize(size_t maxID) 
        {
            m_maxID = maxID;
        }
        
        Entity* CreateEntity() 
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

        void DestroyEntity(Entity* ptrEntity) 
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
		void RegisterComponentManager()
		{
            m_componentManagers[std::type_index(typeid(T))] = std::make_unique<ComponentManager<T>>();
			m_componentManagers[std::type_index(typeid(T))]->Allocate(m_maxID);
		}

        template <typename T, typename... Args>
        void AddComponent(Entity* ptrEntity, Args&&... componentArgs)
        {
            auto it = m_componentManagers.find(std::type_index(typeid(T)));
            if (it != m_componentManagers.end())
            {
                static_cast<ComponentManager<T>*>(it->second.get())->AddComponent(ptrEntity, std::forward<Args>(componentArgs)...);
            }
        }

		template <typename T>
		T* GetComponent (Entity* ptrEntity)
		{
			auto it = m_componentManagers.find(std::type_index(typeid(T)));
			if (it != m_componentManagers.end())
			{
				return static_cast<ComponentManager<T>*>(it->second.get())->GetComponent(ptrEntity);
			}
			else
			{
				throw std::runtime_error("Error: Component Manager not found.");
			}
		}

        template<typename T>
		bool HasComponent(Entity* ptrEntity)
		{
			auto it = m_componentManagers.find(std::type_index(typeid(T)));
			if (it != m_componentManagers.end())
			{
				return static_cast<ComponentManager<T>*>(it->second.get())->HasComponent(ptrEntity);
			}
			return false;
		}

		template<typename T>
        ComponentManager<T>& GetComponentManager()
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
        
		void LoadEntities(std::vector<Entity*>& entities)
		{
			for (auto& manager : m_componentManagers)
			{
				manager.second->LoadComponents(entities);
			}
		}

        void UnloadEntities()
        {
            for (auto& manager : m_componentManagers)
            {
                manager.second->UnloadComponents();
            }
        }

		void ActivateEntities()
		{
			for (auto& manager : m_componentManagers)
			{
				manager.second->ActivateComponents();
			}
		}

		void Activate(Entity* entity)
		{
			for (auto& manager : m_componentManagers)
			{
				manager.second->ActivateComponent(entity);
			}
		}

		void Deactivate(Entity* entity)
		{
			for (auto& manager : m_componentManagers)
			{
				manager.second->DeactivateComponent(entity);
			}
		}

        void DeactivateEntities()
		{
			for (auto& manager : m_componentManagers)
			{
				manager.second->DeactivateComponents();
			}
		}

		template <typename T>
        void RemoveEntity(Entity* ptrEntity)
        {
            auto it = m_componentManagers.find(std::type_index(typeid(T)));
            if (it != m_componentManagers.end())
            {
                static_cast<ComponentManager<T>*>(it->second.get())->RemoveComponent(ptrEntity);
            }
        }

		void Cleanup()
		{
			m_componentManagers.clear();
        }

    private:
        // These must be exposed through API so the client app shares the same objects.
        EntityID m_maxID;
        std::unordered_set<size_t> m_usedIDs;
        std::unordered_map<std::type_index, std::unique_ptr<ComponentManagerBase>> m_componentManagers;

        void releaseID(size_t id)
        {
            m_usedIDs.erase(id);
        }
    };
}