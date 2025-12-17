#pragma once

namespace Engine
{
	// All the possible engine supported body types.

	enum BodyType
	{
		STATIC,
		DYNAMIC,
		KINEMATIC,
		SENSOR
	}; 

	enum Filter
	{
		NONE = 0x00000000, // No collision
		PLAYER = 0x00000002,
		MAP = 0x00000004,
		ALL = 0xFFFFFFFF
	};
}