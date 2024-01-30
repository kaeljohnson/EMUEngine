#pragma once

#include <SDL.h>															// Include external libraries.
#include <SDL_image.h>														//
#include <queue>

#include "WindowManager.h"
#include "RendererManager.h"
#include "Events/EventManager.h"
#include "Events/EventHandlers.h"


namespace Engine
{

	class Application
	{
	private:																	// Declare private memeber variables below.
		bool running;

		EventHandlers m_eventHandlers;
		std::queue<Actions> m_actionsQ;

		WindowManager m_windowManager;
		RendererManager m_rendererManager;									    // Reference to the games RendererManager.
		EventManager m_eventManager;


		// Possible event held arrays? Should not go here.
		bool letterKeys[26] = { false };

		void processActions();


	public:
		Application(const char* appName);								// Declare Game constructor.

		void run();															    // Declare start function.
		void end();																// Declare stop function.


		EventHandlers& getEventHandlers();
		


		Application(const Application&) = delete;												// These four functions are declared and set to "delete" to ensure our game instance cannot be copied or moved. We only want one game instance.
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;
	};

}