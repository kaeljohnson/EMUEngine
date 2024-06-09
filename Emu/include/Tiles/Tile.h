#pragma once

#include "../Core.h"

#include "../Scenes/SceneObject.h"
#include "../Textures/ITexture.h"

namespace Engine
{
	class Tile : public SceneObject
	{
	public:
		EMU_API Tile(const float x, const float y, const float width, const float height, const std::shared_ptr<ITexture> ptrTexture);
		~Tile() = default;
	};
}