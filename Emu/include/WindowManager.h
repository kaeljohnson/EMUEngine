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

		int m_fullscreenWidth;
		int m_fullscreenHeight;

		void free();

	public:
		WindowManager(const char* windowTitle);
		~WindowManager();

		SDLWindow* GetWindow() const;
		const int GetFullscreenWidth() const;
		const int GetFullscreenHeight() const;

		void Resize(const int newWindowWidth, const int newWindowHeight);
		void ToggleFullscreen();

		// Deleted functions to ensure our window manager instance cannot be copied or moved.
		WindowManager(const WindowManager&) = delete;
		WindowManager& operator=(const WindowManager&) = delete;
		WindowManager(WindowManager&&) = delete;
		WindowManager& operator=(WindowManager&&) = delete;
	};
}