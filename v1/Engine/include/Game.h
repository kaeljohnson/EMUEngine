#pragma once

#include <SDL.h>															// Include external libraries.
#include <SDL_image.h>														//

#include "RendererManager.h"												// Include header files.

class Game
{
private:																	// Declare private memeber variables below.
	bool running;																	

	RendererManager& rendererManagerRef;									// Reference to the games RendererManager.

public:
	Game(RendererManager& inputRendererManagerRef);							// Declare Game constructor.

	void handleInput(SDL_Event& e);											// Declare handleInpute function.
	void start();															// Declare start function.
	void stop();															// Declare stop function.

	Game(const Game&) = delete;												// These four functions are declared and set to "delete" to ensure our game instance cannot be copied or moved. We only want one game instance.
	Game& operator=(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(Game&&) = delete;
};