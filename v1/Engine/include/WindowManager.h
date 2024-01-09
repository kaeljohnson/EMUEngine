#pragma once

#include <SDL.h>																				// External library includes.
#include <SDL_image.h>
#include <string>

namespace Engine
{

	class WindowManager																				// Declare WindowManager class.
	{
	private:																						// Declare private member variables below.
		SDL_Window* window;																			// Declare SDL_Window*

	public:																							// Public member functions declared below.
		WindowManager(const char* windowTitle, const uint16_t pixelWidth, const uint16_t pixelHeight);		// Declare WindowManager constructor.
		SDL_Window* getWindow() const;																// Declare getWindow function.
		void free();																				// Declare free function.
	};

}