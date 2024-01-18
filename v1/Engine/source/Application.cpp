#pragma once																		// Ensures there are no units compiled more than once.

#include <SDL.h>																	// External library includes.
#include <stdio.h>

#include "../include/Application.h"													// Header files includes.
#include "../include/RendererManager.h"												//////////////////////
#include "../include/Events/EventManager.h"
#include "../include/Events/EventHandlers.h"

namespace Engine
{

	Application::Application(const char* appName, const uint16_t screenWidth, const uint16_t screenHeight)										// Definition for Game constructor. Note: "<Class>::<Function>" the function on the right side of the "::" comes from the class on the left side.
		: m_windowManager(appName, screenWidth, screenHeight),
		  m_rendererManager(m_windowManager.getWindow()),
		  m_eventManager(),
		  m_eventHandlers(),
		  running(false)																							// Initialization list for the Game constructor. Initialization lists are more optimal than setting variables within the square brackets of the constructor.
	{}

	void Application::run()																	// Definition for The Game class start function.
	{
		SDL_Event e;
		running = true;																	// Once the game is started, the running variable is set to "true" so the game loop runs indefinitely.
		 
		while (running)																	// Game loop.
		{
			while (SDL_PollEvent(&e) != 0)
			{
				dispatchEvent(e);
			}

			m_rendererManager.clearScreen();											// Call to clear screen function on RendererManager object reference. Screen must be cleared once a game loop.
			// m_rendererManager.render(/* Rect */);									// Here is where we will need to render all of our game objects: Player, entities, tiles, etc.
			m_rendererManager.display();												// Call to display function on RendererManager object reference. Once everything is rendered, we must display it on the screen.
		}
	}

	void Application::end()																// Definition for end function on game. Stops the game.
	{
		running = false;
		m_rendererManager.free();
		m_windowManager.free();
	}

	void Application::dispatchEvent(SDL_Event& e)
	{
		// Need to put all the following code into own modules.
		switch (e.type)
		{
		case (SDL_QUIT):
		{
			// Call quit event handler.
			m_eventHandlers.quit();
 			end();
			break;
		}
		case (SDL_WINDOWEVENT):
		{
			// Call window event handler.
			m_eventHandlers.windowEvent();
			if (e.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				// Need to resize camera first.
				m_windowManager.resize(e.window.data1, e.window.data2);
			}
			break;
		}
		case (SDL_KEYDOWN):
		{
			// Call keydown event handler.
			m_eventHandlers.keydown();
			switch (e.key.keysym.sym)
			{
			case (SDLK_k):
			{
				m_windowManager.toggleFullscreen();
			}
			break;
			}
			break;
		}
		case (SDL_KEYUP):
		{
			// Call key up event handler.
			m_eventHandlers.keyup();
			break;
		}
		case (SDL_MOUSEBUTTONDOWN):
		{
			// Call mouse button down event handler.
			m_eventHandlers.mouseButtonDown();
			break;
		}
		case (SDL_MOUSEBUTTONUP):
		{
			// Call mouse button up event handler.
			m_eventHandlers.mouseButtonUp();
			break;
		}
		case (SDL_MOUSEMOTION):
		{
			// Call mouse move event handler.
			m_eventHandlers.mouseMove();
			break;
		}
		case (SDL_MOUSEWHEEL):
		{
			// Call mouse wheel event handler.
			m_eventHandlers.mouseScroll();
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
}