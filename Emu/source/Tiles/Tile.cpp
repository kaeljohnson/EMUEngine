#pragma once

#include "../../include/Tiles/Tile.h"

#include "../../include/Logging/Logger.h"

namespace Engine
{
	Tile::Tile(const size_t id, const BodyType bodyType, const Vector2D<float> position, const Vector2D<float> size, int directionFacing, const bool visible)
		: m_id(id), m_transform(id), m_physicsBody(id, bodyType, false, position, size)
	{}

	Tile::~Tile()
	{
		ENGINE_INFO_D("Destroying tile: " + std::to_string(m_id));
	}
}