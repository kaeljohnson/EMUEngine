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
		EMU_API Tile(const BodyType bodyType, const Vector2D position, const Vector2D size, const std::shared_ptr<ITexture> ptrTexture, int directionFacing);
		~Tile() = default;

	public:
		float x;
		float y;

		float width;
		float height;
	};
}