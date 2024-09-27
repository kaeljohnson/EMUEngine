#pragma once

#include <memory>

#include "../Core.h"
#include "../MathUtil.h"


namespace Engine
{
	class Entity 
	{
	public:
		EMU_API Entity(const Vector2D<float> position, const Vector2D<float> size, int directionFacing, const bool visible);
		~Entity() = default;
	};
}