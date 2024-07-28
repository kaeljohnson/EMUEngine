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
		EMU_API Tile(const BodyType bodyType, const Vector2D<float> position, const Vector2D<float> size, const std::shared_ptr<ITexture> ptrTexture, int directionFacing);
		~Tile() = default;

	public:
		Vector2D<float> m_position;

		// float width;
		// float height;
		Vector2D<float> m_size;
	};
}