#pragma once

#include "Core.h"

#include <string>
#include "MathUtil.h"

namespace Engine
{
	struct Transform
	{
		const size_t m_id;

		Vector2D<float> PrevPosition;
		Vector2D<float> Position;
		Vector2D<float> Velocity;
		Vector2D<float> Dimensions;

		float Rotation;
		float Scale;
		int DirectionFacing;

		EMU_API Transform();
		EMU_API ~Transform();
		EMU_API Transform(const size_t id);
		EMU_API Transform(const size_t id, Vector2D<float> position, Vector2D<float> dimensions, float rotation, float scale, int direction);
	};
}