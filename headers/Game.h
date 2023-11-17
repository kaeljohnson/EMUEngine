#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "RendererManager.h"

class Game
{
private:
	bool running;

	RendererManager& rendererManagerRef;

public:
	Game(RendererManager& inputRendererManagerRef);

	void handleInput(SDL_Event& e);
	void start();
	void stop();

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(Game&&) = delete;
};