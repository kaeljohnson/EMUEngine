#pragma once

#include <stdio.h>
#include <queue>

#include "../include/SDLWrapper/SDLWrapper.h"
#include "../include/Events/Event.h"
#include "../include/WindowManager.h"
#include "../include/Logging/Logger.h"
#include "../include/Layers/Layer.h"

namespace Engine
{
	WindowManager::WindowManager(const char* windowTitle)
		: window(nullptr)
	{
		window = SDL_CREATE_WINDOW(
			windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0,0, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN
		);

		if (window == nullptr)
		{
			ENGINE_CRITICAL("Window not created! SDL_Error: {}", SDL_GetError());
		}

		if (SDL_SET_WINDOW_FULLSCREEN(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
		{
			ENGINE_CRITICAL("Fullscreen mode not set! SDL Error: {}", SDL_GetError());
		}

		// Gotta be an easier way?
		// Is there a way to get the full screen size without toggling fullscreen by default?
		if (SDL_GET_DESKTOP_DISPLAY_MODE(0, &m_displayMode) != 0)
		{
			ENGINE_CRITICAL("Get desktop display mode failed: {}", SDL_GetError());
		}
	}

	SDL_Window* WindowManager::getWindow() const
	{
		return window;
	}

	SDLDisplayMode& WindowManager::getDisplayMode()
	{
		return m_displayMode;
	}

	void WindowManager::resize(const int newWindowWidth, const int newWindowHeight)
	{
		ENGINE_TRACE("{}, {}", newWindowWidth, newWindowHeight);

		// There is still a bug here. The window does not resize properly.
		// Something else changes the window size thats not SDL2.
		SDL_SET_WINDOW_SIZE(window, newWindowWidth, newWindowHeight);
	}

	void WindowManager::toggleFullscreen()
	{
		// Bug here: Figure out why "SDL_WINDOW_FULLSCREEN" does not work.
		// Incompatibility with native video mode?
		bool isFullscreen = SDL_GET_WINDOW_FLAGS(window) & SDL_WINDOW_FULLSCREEN_DESKTOP;

		// Engine should not support resizing the simulation. That is, the pixels per meter should not change.
		// What will happen when the window size changes is that the camera will center on the player, or 
		// whatever object the camera is locked onto, until it hits the edge of the screen.

		if (SDL_SET_WINDOW_FULLSCREEN(window, isFullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP) < 0)
		{
			ENGINE_ERROR("Fullscreen failed! SDL_Error: {}", SDL_GetError());
		}
		else
		{
			// Default behavior for now will be to toggle fullscreen on for client.
			// When the screen is toggled to windowed, the size will be half of the width and height.
			SDL_SET_WINDOW_SIZE(window, m_displayMode.w / 2, m_displayMode.h / 2);
			SDL_SET_WINDOW_POSITION(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		}
	}

	void WindowManager::free()
	{
		ENGINE_INFO("Freeing Window.");
		SDL_DESTROY_WINDOW(window);
		window = nullptr;
	}

	WindowManager::~WindowManager()
	{
		free();
	}
}