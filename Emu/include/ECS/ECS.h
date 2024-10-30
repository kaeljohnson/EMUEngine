#pragma once

#include <string>
#include <random>
#include <sstream>
#include <unordered_set>
#include <typeindex>
#include <unordered_map>

#include "ComponentManager.h"

#include "../Core.h"

namespace Engine
{
	using EntityID = size_t;

    class ECS
    {
    public:
        static void initialize(size_t maxID) 
        {
            ECS::maxID = maxID;
        }
        
        static EntityID CreateEntity() 
        {
            if (usedIDs.size() >= maxID + 1) 
            {
                throw std::runtime_error("Error: All IDs are taken.");
            }

            for (size_t id = 0; id <= maxID; ++id) 
            {
                if (usedIDs.find(id) == usedIDs.end()) 
                {
                    usedIDs.insert(id);
                    return id;
                }
            }

            // This point should never be reached due to the earlier check
            throw std::runtime_error("Error: Unable to generate a new ID.");
        }

        static void DestroyEntity(EntityID entityId) 
        {
            // Ensure the entity is active
            if (usedIDs.find(entityId) != usedIDs.end()) 
            {
                // Recycle the entity ID
                releaseID(entityId);

				for (auto& manager : m_componentManagers)
				{
					manager.second->RemoveComponent(entityId);
				}
            }
        }

		template <typename T>
		static void RegisterComponentManager()
		{
            m_componentManagers[std::type_index(typeid(T))] = std::make_unique<ComponentManager<T>>();
			m_componentManagers[std::type_index(typeid(T))]->Allocate(maxID);
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

        template <typename T, typename... Args>
		static void AddComponent(Args&&... args)
		{
			auto it = m_componentManagers.find(std::type_index(typeid(T)));
			if (it != m_componentManagers.end())
			{
				static_cast<ComponentManager<T>*>(it->second.get())->AddComponent(std::forward<Args>(args)...);
			}
		}

		template <typename T>
        static void RemoveComponent(EntityID entity)
        {
            auto it = m_componentManagers.find(std::type_index(typeid(T)));
            if (it != m_componentManagers.end())
            {
                static_cast<ComponentManager<T>*>(it->second.get())->RemoveComponent(entity);
            }
        }

		static void Cleanup()
		{
			m_componentManagers.clear();
        }

    private:
        // These must be exposed through API so the client app shares the same objects.
        EMU_API static EntityID maxID;
        EMU_API static std::unordered_set<size_t> usedIDs;
        EMU_API static std::unordered_map<std::type_index, std::unique_ptr<ComponentManagerBase>> m_componentManagers;

        static void releaseID(size_t id)
        {
            usedIDs.erase(id);
        }
    };
}