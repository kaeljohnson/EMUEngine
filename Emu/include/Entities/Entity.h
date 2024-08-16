#pragma once

#include "../Core.h"
#include "../MathUtil.h"
#include "../Scenes/SceneObject.h"

namespace Engine
{
	class Entity : public SceneObject
	{
	public:
		EMU_API Entity(const Math::Vector2D<float> position, const Math::Vector2D<float> size, int directionFacing);

		~Entity() = default;
	};
}