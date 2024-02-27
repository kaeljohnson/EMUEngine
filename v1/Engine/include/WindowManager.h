#pragma once

#include <SDL.h>																				// External library includes.
#include <SDL_image.h>
#include <string>
#include <queue>

#include "Events/Event.h"


namespace Engine
{
	// WindowManager class. Manages the window of the application.
	class WindowManager
	{
	private:
		// SDL_Window* pointer. This is the window that will be used to display the application.
		SDL_Window* window;

		// Holds useful information about the display.
		SDL_DisplayMode m_displayMode;

	public:
		// WindowManager constructor.
		WindowManager(const char* windowTitle);

		// Declare WindowManager functions.
		SDL_Window* getWindow() const;
		void processEvents(std::queue<Event>& refEventQ);
		void resize(const int newWindowWidth, const int newWindowHeight);
		void toggleFullscreen();
		void free();
	};
}