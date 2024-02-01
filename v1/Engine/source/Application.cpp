#pragma once																		// Ensures there are no units compiled more than once.
															// External library includes.
#include <stdio.h>
#include <queue>

#include "../include/Application.h"													// Header files includes.
#include "../include/RendererManager.h"

#include "../include/Actions/ActionsEnum.h"

namespace Engine
{

	Application::Application(const char* appName)										// Definition for Game constructor. Note: "<Class>::<Function>" the function on the right side of the "::" comes from the class on the left side.
		: m_windowManager(appName),
		m_rendererManager(m_windowManager.getWindow()),
		m_eventHandlers(),
		m_eventManager(m_eventHandlers, m_actionsQ),
		running(false)																							// Initialization list for the Game constructor. Initialization lists are more optimal than setting variables within the square brackets of the constructor.
	{}

	void Application::run()																	// Definition for The Game class start function.
	{
		running = true;																	// Once the game is started, the running variable is set to "true" so the game loop runs indefinitely.

		while (running)																	// Game loop.
		{
			m_eventManager.handleEvents();

			processActions();

			m_rendererManager.clearScreen();											// Call to clear screen function on RendererManager object reference. Screen must be cleared once a game loop.
			// m_rendererManager.render(/* Rect */);									// Here is where we will need to render all of our game objects: Player, entities, tiles, etc.
			m_rendererManager.display();												// Call to display function on RendererManager object reference. Once everything is rendered, we must display it on the screen.
		}
	}

	void Application::processActions()
	{
		// Should each layer have its own processActions 
		// functions which takes the actions queue and 
		// does what it needs before passing the queue to the next layer?

		while (!m_actionsQ.empty())
		{
			switch (m_actionsQ.front())
			{
			case (QUIT): end(); break;
			case (TOGGLE_FULLSCREEN): m_windowManager.toggleFullscreen(); break;
			case (RESIZE_WINDOW): m_windowManager.resize(100, 100); break;
			default: break;
			}
			m_actionsQ.pop();
		}
	}

	void Application::end()																// Definition for end function on game. Stops the game.
	{
		running = false;
		m_rendererManager.free();
		m_windowManager.free();
	}

	EventHandlers& Application::getEventHandlers()
	{
		return m_eventHandlers;
	}

}