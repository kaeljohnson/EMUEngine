#pragma once

#include <SDL.h>															// Include external libraries.
#include <SDL_image.h>														//

#include "RendererManager.h"												// Include header files.

namespace Engine
{

	class Application
	{
	private:																	// Declare private memeber variables below.
		bool running;

		RendererManager& rendererManagerRef;									// Reference to the games RendererManager.

	public:
		Application(RendererManager& inputRendererManagerRef);							// Declare Game constructor.
		void initializeSDLFeatures();

		void handleInput(SDL_Event& e);											// Declare handleInpute function.
		void start();															// Declare start function.
		void stop();															// Declare stop function.

		Application(const Application&) = delete;												// These four functions are declared and set to "delete" to ensure our game instance cannot be copied or moved. We only want one game instance.
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;
	};

}