#pragma once

#include "../Core.h"

#include "IPhysicsBody.h"
#include "IWorld.h"

namespace Engine
{
	inline EMU_API std::shared_ptr<IPhysicsBody> CreatePhysicsBody(const BodyType bodyType, const float startingXInMeters, const float startingYInMeters,
		const float widthInMeters, const float heightInMeters, const float density, const float friction, const float angle,
		const float restitution, const float restitutionThreshold, const bool collidable, const bool fixed, const bool visible);

	inline EMU_API std::unique_ptr<IWorld> CreateWorld(const float gravityX, const float gravityY, const int velocityIterations, const int positionIterations);
}