#pragma once


#include "../../include/Textures/ITexture.h"
#include "../../include/Tiles/Tile.h"

namespace Engine
{
	Tile::Tile(const float x, const float y, const float width, const float height, const std::shared_ptr<ITexture> ptrTexture)
		: SceneObject(STATIC, true, x, y, width, height, ptrTexture)
	{
	}
}