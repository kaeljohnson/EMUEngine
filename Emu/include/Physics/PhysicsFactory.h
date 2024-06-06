#pragma once

#include "IPhysicsBody.h"
#include "IWorld.h"

namespace Engine
{
	std::shared_ptr<IPhysicsBody> CreatePhysicsBody(const BodyType bodyType, const bool fixed, const float startingXInMeters, const float startingYInMeters,
		const float widthInMeters, const float heightInMeters);

	std::unique_ptr<IWorld> CreateWorld(const float gravityX, const float gravityY, const int velocityIterations, const int positionIterations);
}