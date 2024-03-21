#pragma once

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
} 