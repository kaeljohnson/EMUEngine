#pragma once

#include <memory>

#include "../../include/Physics/PhysicsFactory.h"
#include "../../include/Physics/Box.h"
#include "../../include/Physics/World.h"

namespace Engine
{
    // Need to have less stuff in the constructor. User can set the rest of the values later.
    std::shared_ptr<IPhysicsBody> CreatePhysicsBody(const BodyType bodyType, const bool fixed, const float startingXInMeters, const float startingYInMeters,
        const float widthInMeters, const float heightInMeters)
    {
        return std::make_shared<Box>(bodyType, fixed, startingXInMeters, startingYInMeters, widthInMeters, heightInMeters);
    }

    std::unique_ptr<IWorld> CreateWorld(const float gravityX, const float gravityY, const int velocityIterations, const int positionIterations)
    {
        return std::make_unique<World>(gravityX, gravityY, velocityIterations, positionIterations);
    }
}