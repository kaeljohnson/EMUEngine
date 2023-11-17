#pragma once

#include <SDL.h>

#include "../headers/Game.h"
#include "../headers/RendererManager.h"

Game::Game(RendererManager& inputRendererManagerRef) 
	: rendererManagerRef(inputRendererManagerRef), running(false)
{}

void Game::handleInput(SDL_Event& e)
{
	if (e.type == SDL_QUIT)
	{
		stop();
	}
	else if (e.key.keysym.sym == SDLK_ESCAPE)
	{
		stop();
	}
}

void Game::start()
{
	SDL_Event e;

	running = true;

	while (running)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			handleInput(e);
		}

		rendererManagerRef.clearScreen();
		// rendererManagerRef.render(/* Rect */);
		rendererManagerRef.display();
	}
}

void Game::stop()
{
	running = false;
}