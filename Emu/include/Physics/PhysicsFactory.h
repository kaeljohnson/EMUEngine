#pragma once

#include "../Core.h"

#include "PhysicsBody.h"
#include "World.h"

namespace Engine
{
	std::shared_ptr<PhysicsBody> CreatePhysicsBody(const BodyType bodyType, const bool fixed, const Vector2D<float> position, const Vector2D<float> size);

	std::unique_ptr<World> CreateWorld(const float gravityX, const float gravityY, const int velocityIterations, const int positionIterations);
}