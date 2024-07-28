#pragma once

#include "../../include/Textures/ITexture.h"
#include "../../include/Tiles/Tile.h"

namespace Engine
{
	Tile::Tile(const BodyType bodyType, const Vector2D<float> position, const Vector2D<float> size, const std::shared_ptr<ITexture> ptrTexture, int directionFacing)
		: m_position(position), m_size(size), SceneObject(bodyType, true, position, size, ptrTexture, directionFacing) {}
}