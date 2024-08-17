#pragma once

#include "../Core.h"

namespace Engine
{
	struct Screen
	{
		EMU_API static Vector2D<int> SCREEN_SIZE;
		EMU_API static Vector2D<int> VIRTUAL_SIZE;
		EMU_API static Vector2D<float> SCALE;
		EMU_API static float SCALE_CONSTANT;

		EMU_API static Vector2D<int> VIEWPORT_SIZE;
		EMU_API static Vector2D<int> VIEWPORT_POSITION;
	};
}