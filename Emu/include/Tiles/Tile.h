#pragma once

#include "../Core.h"

#include "../Scenes/SceneObject.h"
#include "../Physics/BodyTypes.h"

namespace Engine
{
	class Tile : public SceneObject
	{
	public:
		EMU_API Tile(const BodyType bodyType, const Math::Vector2D<float> position, const Math::Vector2D<float> size, int directionFacing);
		~Tile() = default;

	public:
		Math::Vector2D<float> m_position;
		Math::Vector2D<float> m_size;
	};
}