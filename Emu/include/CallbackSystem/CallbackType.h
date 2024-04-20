#pragma once

namespace Engine
{
	enum Type
	{
		// World interaction
		AddToWorld = 0,

		// Application interaction
		EndApplication,
		ToggleFullscreen,
		ResizeWindow,

		// Texture interaction
		LoadDefaultTexture,
	};
}