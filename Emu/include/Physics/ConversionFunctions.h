#pragma once

#ifndef M_PIf
#define M_PIf 3.14159265358979323846f
#endif

namespace Engine
{
    inline float degreesToRadians(float degrees)
    {
		return degrees * M_PIf / 180.0f;
	}

    inline float radiansToDegrees(float radians)
    {
        return radians * 180.0f / M_PIf;
    }
} 