#pragma once

#include "../../include/Transform.h"
#include "../../include/Physics/PhysicsBody.h"
#include "../../include/Updatable/Updatable.h"

#include "../../include/ECS/ComponentManager.h"

#define INSTANTIATE_GET_MANAGER(T) template ComponentManager<T>& ComponentManagerRegistry::GetManager<T>();

namespace Engine
{
    
    template <typename T>
    ComponentManager<T>& ComponentManagerRegistry::GetManager()
    {
        static ComponentManager<T> manager;
        return manager;
    }

    template EMU_API ComponentManager<Transform>& ComponentManagerRegistry::GetManager<Transform>();
    template EMU_API ComponentManager<PhysicsBody>& ComponentManagerRegistry::GetManager<PhysicsBody>();
    template EMU_API ComponentManager<Updatable>& ComponentManagerRegistry::GetManager<Updatable>();


    // Explicit template instantiation for the types you need
    /*INSTANTIATE_GET_MANAGER(Transform)
        INSTANTIATE_GET_MANAGER(PhysicsBody)
        INSTANTIATE_GET_MANAGER(Updatable)*/
        // Add more instantiations as needed
        


}