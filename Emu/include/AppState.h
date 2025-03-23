#pragma once

namespace Engine
{
	// Static class to store the current state of the application.
	// Whether a scene is playing, paused, or stopped, etc.
	struct AppState
	{
		static bool IN_SCENE;
		static bool IS_PAUSED;
		static bool IS_STOPPED;
		static bool IS_LOADING;
	};

	inline bool AppState::IN_SCENE = false;
	inline bool AppState::IS_PAUSED = false;
	inline bool AppState::IS_STOPPED = false;
	inline bool AppState::IS_LOADING = false;
}