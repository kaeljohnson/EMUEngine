#pragma once

#include "../include/SDLWrapper/SDLWrapper.h"


namespace Engine
{
	// WindowManager class. Manages the window of the application.
	class WindowManager
	{
	private:
		// SDL_Window* pointer. This is the window that will be used to display the application.
		SDLWindow* window;

		// Holds useful information about the display.
		SDLDisplayMode m_displayMode;

	public:
		WindowManager(const char* windowTitle);
		~WindowManager();

		SDLWindow* getWindow() const;
		void resize(const int newWindowWidth, const int newWindowHeight);
		void toggleFullscreen();
		void free();

		// Deleted functions to ensure our window manager instance cannot be copied or moved.
		WindowManager(const WindowManager&) = delete;
		WindowManager& operator=(const WindowManager&) = delete;
		WindowManager(WindowManager&&) = delete;
		WindowManager& operator=(WindowManager&&) = delete;
	};
}