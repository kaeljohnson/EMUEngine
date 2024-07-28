#pragma once

#include "../../include/Textures/ITexture.h"
#include "../../include/Tiles/Tile.h"

namespace Engine
{
	Tile::Tile(const BodyType bodyType, const Vector2D<float> position, const Vector2D<float> size, const std::shared_ptr<ITexture> ptrTexture, int directionFacing)
		: x(x), y(y), width(width), height(height), SceneObject(bodyType, true, position, size, ptrTexture, directionFacing) {}
}