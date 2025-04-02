#pragma once

#include "../Core.h"

namespace Engine
{
	// All the possible engine supported body types.

	enum EMU_API BodyType
	{
		STATIC,
		DYNAMIC,
		KINEMATIC,
		SENSOR
	}; 

	enum EMU_API Filter
	{
		NONE = 0x00000000, // No collision
		PLAYER = 0x00000002,
		MAP = 0x00000004,
		ALL = 0xFFFFFFFF
	};
}