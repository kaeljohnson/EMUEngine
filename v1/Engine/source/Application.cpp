#pragma once																		// Ensures there are no units compiled more than once.

#include <SDL.h>																	// External library includes.
#include <stdio.h>

#include "../include/Application.h"													// Header files includes.
#include "../include/RendererManager.h"												//////////////////////

namespace Engine
{

	Application::Application(const char* appName, const uint16_t screenWidth, const uint16_t screenHeight)										// Definition for Game constructor. Note: "<Class>::<Function>" the function on the right side of the "::" comes from the class on the left side.
		: m_windowManager(appName, screenWidth, screenHeight),
		  m_rendererManager(m_windowManager.getWindow()),
		  running(false)																							// Initialization list for the Game constructor. Initialization lists are more optimal than setting variables within the square brackets of the constructor.
	{}

	void Application::handleInput(SDL_Event& e)												// Function definition for handling input. On each interation of the game loop, user input will have to be interpereted.
	{
		// Need to put all the following code into own modules.
		switch (e.type)
		{
			case (SDL_QUIT):
			{
				printf("end1.");
				end();
				break;
			}
			case (SDL_WINDOWEVENT):
			{

				if (e.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					// Need to resize camera first.
					m_windowManager.resize(e.window.data1, e.window.data2);
				}
				break;
			} 
			case (SDL_KEYDOWN):
			{
				switch (e.key.keysym.sym)
				{
					case (SDLK_k):
					{
						// Bug here: Figure out why "SDL_WINDOW_FULLSCREEN" does not work.
						// Incompatibility with native video mode?
						bool isFullscreen = SDL_GetWindowFlags(m_windowManager.getWindow()) & SDL_WINDOW_FULLSCREEN_DESKTOP;
						if (SDL_SetWindowFullscreen(m_windowManager.getWindow(), isFullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP) < 0)
						{
							printf("Fullscreen failed! SDL_Error: %s\n", SDL_GetError());
						}
						else
						{
							SDL_SetWindowSize(m_windowManager.getWindow(), 1800, 900);	// Go back to user set size?
							SDL_SetWindowPosition(m_windowManager.getWindow(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
						}
						// SDL_SetWindowDisplayMode(m_windowManager.getWindow(), NULL);
					}
					break;
				}
				break;
			}
			default:
			{
				if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					printf("end2");
					// bug here: This function is getting called seemingly randomly.
					//end();
				}
				break;
			}
		}
		
		
	}

	void Application::run()																	// Definition for The Game class start function.
	{
		SDL_Event e;																	// Declare an event variable of type "SDL_Event" so we can track SDL events while game is running. Note: Not quite sure how SDL_Events work under the hood.

		running = true;																	// Once the game is started, the running variable is set to "true" so the game loop runs indefinitely.

		while (running)																	// Game loop.
		{
			while (SDL_PollEvent(&e) != 0)												// SDL_Event polling. If there is no even (SDL_PollEvent(&e) == 0), leave the polling loop and continue with game.
			{
				handleInput(e);															// Call handleInput function with event to determine what to do with the input event.
			}

			m_rendererManager.clearScreen();											// Call to clear screen function on RendererManager object reference. Screen must be cleared once a game loop.
			// rendererManagerRef.render(/* Rect */);									// Here is where we will need to render all of our game objects: Player, entities, tiles, etc.
			m_rendererManager.display();												// Call to display function on RendererManager object reference. Once everything is rendered, we must display it on the screen.
		}
	}

	void Application::end()																// Definition for end function on game. Stops the game.
	{
		running = false;
		m_rendererManager.free();
		m_windowManager.free();
	}
}