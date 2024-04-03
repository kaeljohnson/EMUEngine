#pragma once

namespace Engine
{
	enum Type
	{
		// World interaction
		AddToWorld = 0,
		RemoveFromWorld,

		// Application interaction
		EndApplication,
		ToggleFullscreen,
		ResizeWindow,
	};
}