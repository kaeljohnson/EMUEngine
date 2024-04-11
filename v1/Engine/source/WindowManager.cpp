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
			windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0,0, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP
		);

		if (window == nullptr)
		{
			ENGINE_CRITICAL("Window not created! SDL_Error: {}", SDL_GetError());
		}

		// Gotta be an easier way?
		// Is there a way to get the full screen size without toggling fullscreen by default?
		SDL_DisplayMode displayMode;
		if (SDL_GET_DESKTOP_DISPLAY_MODE(0, &displayMode) != 0)
		{
			ENGINE_CRITICAL_D("Get desktop display mode failed: {}", SDL_GetError());
		}

		m_fullscreenWidth = displayMode.w;
		m_fullscreenHeight = displayMode.h;
	}

	SDL_Window* WindowManager::getWindow() const
	{
		return window;
	}

	const int WindowManager::GetFullscreenWidth() const
	{
		return m_fullscreenWidth;
	}

	const int WindowManager::GetFullscreenHeight() const
	{
		return m_fullscreenHeight;
	}

	void WindowManager::resize(const int newWindowWidth, const int newWindowHeight)
	{
		ENGINE_TRACE_D("{}, {}", newWindowWidth, newWindowHeight);

		if (newWindowWidth < m_fullscreenWidth / 2 && newWindowHeight < m_fullscreenHeight / 2)
		{
			SDL_SET_WINDOW_SIZE(window, m_fullscreenWidth / 2, m_fullscreenHeight / 2);
		}
		else if (newWindowWidth < m_fullscreenWidth / 2) 
		{ 
			SDL_SET_WINDOW_SIZE(window, m_fullscreenWidth / 2, newWindowHeight); 
		}
		else if (newWindowHeight < m_fullscreenHeight / 2) 
		{ 
			SDL_SET_WINDOW_SIZE(window, newWindowWidth, m_fullscreenHeight / 2); 
		}
		else
		{
			// Let OS handle resize.
		}

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
			SDL_SET_WINDOW_SIZE(window, m_fullscreenWidth / 2, m_fullscreenHeight / 2);
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