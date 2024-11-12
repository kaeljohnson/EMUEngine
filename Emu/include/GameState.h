#pragma once

namespace Engine
{
	// Static class to store the current state of the game.
	// Whether a scene is playing, paused, or stopped, etc.
	struct GameState
	{
		static bool IN_SCENE;
	};

	inline bool GameState::IN_SCENE = false;
}