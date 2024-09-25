#pragma once

#include "../Core.h"

#include "../Transform.h"
#include "../Physics/PhysicsBody.h"
#include "../Physics/BodyTypes.h"

namespace Engine
{
	class Tile
	{
	public:
		EMU_API Tile(const size_t id, const BodyType bodyType, const Vector2D<float> position, const Vector2D<float> size, int directionFacing, const bool visible);
		EMU_API ~Tile();

	public:
		const size_t m_id;
		Transform m_transform;
		PhysicsBody m_physicsBody;
	};
}