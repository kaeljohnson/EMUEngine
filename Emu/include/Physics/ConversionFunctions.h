#pragma once

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Engine
{
    inline double degreesToRadians(double degrees)
    {
		return degrees * M_PI / 180.0f;
	}

    inline double radiansToDegrees(double radians)
    {
        return radians * 180.0 / M_PI;
    }
} 