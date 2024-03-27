#pragma once

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Engine
{
	inline const float PIXELS_PER_METER = 10.0f;

    // Convert a length from pixels to meters.
    inline float pixelsToMeters(float pixels)
    {
        return pixels / PIXELS_PER_METER;
    }

    // Convert a length from meters to pixels.
    inline int metersToPixels(float meters)
    {
        // Can't have half a pixel, so round to the nearest pixel.
        return static_cast<int>(round(meters * PIXELS_PER_METER));
    }

    inline double degreesToRadians(double degrees)
    {
		return degrees * M_PI / 180.0f;
	}

    inline double radiansToDegrees(double radians)
    {
        return radians * 180.0 / M_PI;
    }
} 