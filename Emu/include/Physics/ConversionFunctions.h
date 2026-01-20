#pragma once

#ifndef M_PIf
#define M_PIf 3.14159265358979323846f
#endif

namespace Engine
{
    /**
	* @brief Converts degrees to radians.
    * 
	* @param degrees Angle in degrees.
    * 
	* @return Angle in radians.
    */
    inline float degreesToRadians(float degrees)
    {
		return degrees * M_PIf / 180.0f;
	}

	/**
    * 
	* @brief Converts radians to degrees.
    * 
	* @param radians Angle in radians.
    * 
	* @return Angle in degrees.
	*/
    inline float radiansToDegrees(float radians)
    {
        return radians * 180.0f / M_PIf;
    }
} 