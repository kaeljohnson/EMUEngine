#pragma once

#include "IPhysicsBody.h"
#include "IWorld.h"
#include "../Core.h"

namespace Engine
{
	EMU_API IPhysicsBody* CreatePhysicsBody(const BodyType bodyType, const float startingXInMeters, const float startingYInMeters,
		const float widthInMeters, const float heightInMeters, const float density, const float friction, const float angle,
		const float restitution, const float restitutionThreshold, const bool collidable, const bool fixed, const bool visible);

	EMU_API std::unique_ptr<IWorld> CreateWorld(const float gravityX, const float gravityY, const float deltaTime, const int velocityIterations, const int positionIterations);
}