#pragma once

#include <queue>

#include "WindowManager.h"
#include "RendererManager.h"
#include "Events/EventManager.h" 
#include "Events/Event.h"

namespace Engine
{
	class Application
	{
	private:																	// Declare private memeber variables below.
		bool running;

		std::queue<Event> m_eventQ;

		WindowManager m_windowManager;
		RendererManager m_rendererManager;									    // Reference to the games RendererManager.
		EventManager m_eventManager;

		void processActions();

	public:
		Application(const char* appName);								// Declare Game constructor.

		void run();															    // Declare start function.
		void end();																// Declare stop function.

		Application(const Application&) = delete;												// These four functions are declared and set to "delete" to ensure our game instance cannot be copied or moved. We only want one game instance.
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;
	};
}