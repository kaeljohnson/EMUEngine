#pragma once																		// Ensures there are no units compiled more than once.

#include <SDL.h>																	// External library includes.

#include "../include/Game.h"														// Header files includes.
#include "../include/RendererManager.h"												//////////////////////

Game::Game(RendererManager& inputRendererManagerRef)								// Definition for Game constructor. Note: "<Class>::<Function>" the function on the right side of the "::" comes from the class on the left side.
	: rendererManagerRef(inputRendererManagerRef), running(false)					// Initialization list for the Game constructor. Initialization lists are more optimal than setting variables within the square brackets of the constructor.
{}

void Game::handleInput(SDL_Event& e)												// Function definition for handling input. On each interation of the game loop, user input will have to be interpereted.
{
	if (e.type == SDL_QUIT)															// If the SDL_event reference has a value of "SDL_QUIT" the game will stop.
	{
		stop();
	}
	else if (e.key.keysym.sym == SDLK_ESCAPE)										// If the SDL_event reference has a value of "SDLK_ESCAPE" the game will stop.
	{
		stop();
	}
}

void Game::start()																	// Definition for The Game class start function.
{
	SDL_Event e;																	// Declare an event variable of type "SDL_Event" so we can track SDL events while game is running. Note: Not quite sure how SDL_Events work under the hood.

	running = true;																	// Once the game is started, the running variable is set to "true" so the game loop runs indefinitely.

	while (running)																	// Game loop.
	{
		while (SDL_PollEvent(&e) != 0)												// SDL_Event polling. If there is no even (SDL_PollEvent(&e) == 0), leave the polling loop and continue with game.
		{
			handleInput(e);															// Call handleInput function with event to determine what to do with the input event.
		}

		rendererManagerRef.clearScreen();											// Call to clear screen function on RendererManager object reference. Screen must be cleared once a game loop.
		// rendererManagerRef.render(/* Rect */);									// Here is where we will need to render all of our game objects: Player, entities, tiles, etc.
		rendererManagerRef.display();												// Call to display function on RendererManager object reference. Once everything is rendered, we must display it on the screen.
	}
}

void Game::stop()																	// Definition for stop function on game. Stops the game.
{
	running = false;
}