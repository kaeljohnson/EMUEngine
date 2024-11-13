#pragma once

#include "ECS/Component.h"

#include "Core.h"

#include <string>
#include "MathUtil.h"

namespace Engine
{
	struct Transform : public Component
	{
		Vector2D<float> PrevPosition;
		Vector2D<float> Position;
		Vector2D<float> Velocity;
		Vector2D<float> Dimensions;

		int ZIndex;
		float Rotation;
		float Scale;
		int DirectionFacing;

		EMU_API ~Transform();
		EMU_API Transform(Entity* entity);
		EMU_API Transform(Entity* entity, Vector2D<float> position, Vector2D<float> dimensions, float rotation, float scale, int direction);
	};
}