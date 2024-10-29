#pragma once

#include "Core.h"

namespace Engine
{
	struct Time
	{
		EMU_API inline static const float GetInterpolationFactor() { return INTERPOLATION_FACTOR; }

		static float INTERPOLATION_FACTOR;
	};
}