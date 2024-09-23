#pragma once

#include "../../include/Tiles/Tile.h"

namespace Engine
{
	Tile::Tile(const BodyType bodyType, const Vector2D<float> position, const Vector2D<float> size, int directionFacing, const bool visible)
		: SceneObject(bodyType, true, position, size, directionFacing, visible), m_transform() 
	{
		m_transform = Transform(uuid);
	}
}