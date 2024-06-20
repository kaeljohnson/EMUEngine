#pragma once

#include "../Core.h"

#include "../Scenes/SceneObject.h"
#include "../Textures/ITexture.h"
#include "../Physics/BodyTypes.h"

namespace Engine
{
	class Tile : public SceneObject
	{
	public:
		EMU_API Tile(const BodyType bodyType, const float x, const float y, const float width, const float height, const std::shared_ptr<ITexture> ptrTexture);
		~Tile() = default;

		float x;
		float y;

		float width;
		float height;
	};
}