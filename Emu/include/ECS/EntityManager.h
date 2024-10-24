#pragma once

#include <string>
#include <random>
#include <sstream>
#include <unordered_set>

#include "../Core.h"

// Can't have this since its depending on exterior files to ECS...
#include "../Updatable/Updatable.h"
#include "../Physics/PhysicsBody.h"
#include "../Transform.h"

#include "ComponentManager.h"

namespace Engine
{
	using Entity = size_t;

    class EntityManager
    {
    public:
        static void initialize(size_t maxID) 
        {
            EntityManager::maxID = maxID;
        }

        EMU_API static Entity CreateEntity() 
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

        static void DestroyEntity(Entity entityId) 
        {
            // Ensure the entity is active
            if (usedIDs.find(entityId) != usedIDs.end()) 
            {
                // Recycle the entity ID
                releaseID(entityId);

                // Should come up with a better way to update components in their respective managers than just calling all the 
				// possible managers here. This is a temporary solution.
                ComponentManagerRegistry::GetManager<Updatable>().RemoveComponent(entityId);
                ComponentManagerRegistry::GetManager<PhysicsBody>().RemoveComponent(entityId);
                ComponentManagerRegistry::GetManager<Transform>().RemoveComponent(entityId);
            }
        }

    private:
        static Entity maxID;
        static std::unordered_set<size_t> usedIDs;

        static void releaseID(size_t id)
        {
            usedIDs.erase(id);
        }
    };
}