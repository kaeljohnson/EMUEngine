#pragma once

namespace Engine
{

	// Callback types which are used to create and trigger callbacks.

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

		// Scene interaction
		SetSimulation,
	};
}