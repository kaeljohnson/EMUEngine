#pragma once

#include <memory>
#include "../../include/Physics/PhysicsFactory.h"
#include "../../include/Physics/Box.h"
#include "../../include/Physics/World.h"

namespace Engine
{
   std::shared_ptr<IPhysicsBody> CreatePhysicsBody(const BodyType bodyType, const float startingXInMeters, const float startingYInMeters,
        const float widthInMeters, const float heightInMeters, const float density, const float friction, const float angle,
        const float restitution, const float restitutionThreshold, const bool collidable, const bool fixed, const bool visible)
    {
        return std::make_shared<Box>(bodyType, startingXInMeters, startingYInMeters, widthInMeters,
            heightInMeters, density, friction, angle, restitution, restitutionThreshold, collidable, fixed, visible);
    }

    std::unique_ptr<IWorld> CreateWorld(const float gravityX, const float gravityY, const float deltaTime, const int velocityIterations, const int positionIterations)
    {
        return std::make_unique<World>(gravityX, gravityY, deltaTime, velocityIterations, positionIterations);
    }
}