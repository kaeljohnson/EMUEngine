#pragma once

#include "Core.h"

namespace Engine
{
	struct Time
	{
		EMU_API inline static const float GetInterpolationFactor() { return INTERPOLATION_FACTOR; }
		EMU_API inline static const float GetTimeStep() { return TIME_STEP; }

		EMU_API inline static void SetInterpolationFactor(const float interpolationFactor) { INTERPOLATION_FACTOR = interpolationFactor; }
		EMU_API inline static void SetTimeStep(const float timeStep) { TIME_STEP = timeStep; }


	private:
		static float INTERPOLATION_FACTOR;

		static float TIME_STEP;
	};

	inline float Time::INTERPOLATION_FACTOR = 0.0f;
	inline float Time::TIME_STEP = 1.0f / 60.0f;
}