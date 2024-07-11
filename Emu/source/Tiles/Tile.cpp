#pragma once

#include "../../include/Textures/ITexture.h"
#include "../../include/Tiles/Tile.h"

namespace Engine
{
	Tile::Tile(const BodyType bodyType, const float x, const float y, const float width, const float height, const std::shared_ptr<ITexture> ptrTexture, int directionFacing)
		: x(x), y(y), width(width), height(height), SceneObject(bodyType, true, x, y, width, height, ptrTexture, directionFacing) {}
}