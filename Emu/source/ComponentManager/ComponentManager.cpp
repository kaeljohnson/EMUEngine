#pragma once

#include "../../include/Transform.h"
#include "../../include/Physics/PhysicsBody.h"
#include "../../include/Updatable/Updatable.h"

#include "../../include/ECS/EntityManager.h"

namespace Engine
{
    std::unordered_map<std::type_index, std::unique_ptr<Engine::ComponentManagerBase>> EntityManager::m_componentManagers;

    /*template <typename T>
    ComponentManager<T>& EntityManager::GetManager()
    {
        static ComponentManager<T> manager;
        return manager;
    }*/

    template EMU_API ComponentManager<Transform>& EntityManager::GetComponentManager<Transform>();
    template EMU_API ComponentManager<PhysicsBody>& EntityManager::GetComponentManager<PhysicsBody>();
    template EMU_API ComponentManager<Updatable>& EntityManager::GetComponentManager<Updatable>();


    // Explicit template instantiation for the types you need
    /*INSTANTIATE_GET_MANAGER(Transform)
        INSTANTIATE_GET_MANAGER(PhysicsBody)
        INSTANTIATE_GET_MANAGER(Updatable)*/
        // Add more instantiations as needed
        


}