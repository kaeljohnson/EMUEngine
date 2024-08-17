#pragma once

#include "../../include/Tiles/Tile.h"

namespace Engine
{
	Tile::Tile(const BodyType bodyType, const Vector2D<float> position, const Vector2D<float> size, int directionFacing)
		: m_position(position), m_size(size), SceneObject(bodyType, true, position, size, directionFacing) {}
}