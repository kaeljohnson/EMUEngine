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
		EMU_API Tile(Entity& entity, const BodyType bodyType, const Vector2D<float> position, 
			const Vector2D<float> size, int directionFacing, const bool visible);
		EMU_API ~Tile();

	public:
		const int m_id;
		Transform m_transform;
		PhysicsBody m_physicsBody;
	};
}