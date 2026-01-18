#pragma once

namespace Engine
{
	/** @struct AppState
	* 
	* @brief Static struct that stores information as to whether a scene is playing, paused, or stopped, etc.
	*/
	struct AppState
	{
		static bool IN_SCENE;	/// Is the application currently in a scene?
		static bool IS_PAUSED;	/// Is the application currently paused?
		static bool IS_STOPPED;	/// Is the application currently stopped?
		static bool IS_LOADING;	/// Is the application currently loading?
	};

	inline bool AppState::IN_SCENE = false;
	inline bool AppState::IS_PAUSED = false;
	inline bool AppState::IS_STOPPED = false;
	inline bool AppState::IS_LOADING = false;
}