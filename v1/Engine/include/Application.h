#pragma once

#include <SDL.h>															// Include external libraries.
#include <SDL_image.h>														//

#include "WindowManager.h"
#include "RendererManager.h"												// Include header files.


namespace Engine
{

	class Application
	{
	private:																	// Declare private memeber variables below.
		bool running;

		WindowManager m_windowManager;
		RendererManager m_rendererManager;									// Reference to the games RendererManager.
		

	public:
		Application(const char* appName, const uint16_t screenWidth, const uint16_t screenHeight);								// Declare Game constructor.

		void handleInput(SDL_Event& e);											// Declare handleInpute function.
		void run();															// Declare start function.
		void end();																// Declare stop function.

		Application(const Application&) = delete;												// These four functions are declared and set to "delete" to ensure our game instance cannot be copied or moved. We only want one game instance.
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;
	};

}