#pragma once

#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <queue>

#include "../include/Events/Event.h"
#include "../include/WindowManager.h"
#include "../include/Logging/Logger.h"
#include "../include/Layers/Layer.h"

namespace Engine
{
	WindowManager::WindowManager(const char* windowTitle)
		: window(nullptr)
	{
		window = SDL_CreateWindow(
			windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0,0, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN
		);

		if (window == nullptr)
		{
			ENGINE_CRITICAL("Window not created! SDL_Error: {}", SDL_GetError());
		}

		if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
		{
			ENGINE_CRITICAL("Fullscreen mode not set! SDL Error: {}", SDL_GetError());
		}

		// Gotta be an easier way?
		// Is there a way to get the full screen size without toggling fullscreen by default?
		if (SDL_GetDesktopDisplayMode(0, &m_displayMode) != 0)
		{
			ENGINE_CRITICAL("Get desktop display mode failed: {}", SDL_GetError());
		}
	}

	SDL_Window* WindowManager::getWindow() const
	{
		return window;
	}

	void WindowManager::resize(const int newWindowWidth, const int newWindowHeight)
	{
		ENGINE_TRACE("{}, {}", newWindowWidth, newWindowHeight);
		SDL_SetWindowSize(window, newWindowWidth, newWindowHeight);
	}

	void WindowManager::toggleFullscreen()
	{
		// Bug here: Figure out why "SDL_WINDOW_FULLSCREEN" does not work.
		// Incompatibility with native video mode?
		bool isFullscreen = SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP;
		if (SDL_SetWindowFullscreen(window, isFullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP) < 0)
		{
			ENGINE_ERROR("Fullscreen failed! SDL_Error: {}", SDL_GetError());
		}
		else
		{
			// Default behavior for now will be to toggle fullscreen on for client.
			// When the screen is toggled to windowed, the size will be half of the width and height.
			SDL_SetWindowSize(window, m_displayMode.w / 2, m_displayMode.h / 2);
			SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		}
	}

	void WindowManager::free()
	{
		ENGINE_INFO("Freeing Window.");
		SDL_DestroyWindow(window);
		window = nullptr;
	}

	WindowManager::~WindowManager()
	{
		free();
	}
}