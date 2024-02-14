#pragma once

#include <stdio.h>																										// External library includes.
#include <SDL.h>																										//
#include <SDL_image.h>																											//
#include <queue>

#include "../include/Events/Event.h"
#include "../include/WindowManager.h"																					// Include header files.

namespace Engine
{

	WindowManager::WindowManager(const char* windowTitle)									// Definition of WindowManager constructor.
		: window(nullptr)																									// Initialization list for WindowManager constructor.
	{
		window = SDL_CreateWindow(																							// Call to SDL_CreateWindow function. Hover over function to understand arguments.
			windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0,0, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN
		);

		if (window == nullptr)																								// If the window is still a nullptr, print an error.
		{
			printf("Window not created! SDL_Error: %s\n", SDL_GetError());
		}

		if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
		{
			printf("Fullscreen mode not set! SDL Error: %s\n", SDL_GetError());
		}
	}

	void WindowManager::processEvents(std::queue<Event>& refEventQ)
	{
		// Call window manager callbacks which are user defined?
		size_t eventQCounter = 0;
		bool doneProcessingEvents = refEventQ.empty();
		while (!doneProcessingEvents)
		{
			Event& currentEvent = refEventQ.front();

			switch (currentEvent.m_eventType)
			{
				case (F_KEY_DOWN): toggleFullscreen(); refEventQ.pop(); break;
				case (RESIZE_WINDOW): resize(currentEvent.m_xPos, currentEvent.m_yPos); refEventQ.pop(); break;
				default: ++eventQCounter; break;
			}

			doneProcessingEvents = refEventQ.empty() || eventQCounter > refEventQ.size();
		}
	}

	SDL_Window* WindowManager::getWindow() const																			// Getter for window stored in WindowManager.
	{
		return window;
	}

	void WindowManager::resize(const int newWindowWidth, const int newWindowHeight)
	{
		printf("%d, %d", newWindowWidth, newWindowHeight);
		SDL_SetWindowSize(window, newWindowWidth, newWindowHeight);
	}

	void WindowManager::toggleFullscreen()
	{
		// Bug here: Figure out why "SDL_WINDOW_FULLSCREEN" does not work.
		// Incompatibility with native video mode?
		bool isFullscreen = SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP;
		if (SDL_SetWindowFullscreen(window, isFullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP) < 0)
		{
			printf("Fullscreen failed! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			SDL_SetWindowSize(window, 1800, 900);	// Go back to user set size?
			SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		}
	}

	void WindowManager::free()																								// Definition for free function in WindowManager.
	{
		SDL_DestroyWindow(window);																							// Free dynamically allocated window.
		window = nullptr;																									// Make sure window is not still attached to the freed memory.
	}

}