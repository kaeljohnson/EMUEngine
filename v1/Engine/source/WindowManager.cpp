#pragma once

#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <queue>

#include "../include/Events/Event.h"
#include "../include/WindowManager.h"

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
			printf("Window not created! SDL_Error: %s\n", SDL_GetError());
		}

		if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
		{
			printf("Fullscreen mode not set! SDL Error: %s\n", SDL_GetError());
		}
	}

	void WindowManager::processEvents(std::queue<Event>& refEventQ)
	{
		// Better way to do this function?

		// Call window manager callbacks which are user defined?
		size_t eventQCounter = 0;
		bool doneProcessingEvents = refEventQ.empty();
		while (!doneProcessingEvents)
		{
			const Event currentEvent = refEventQ.front();
			refEventQ.pop();

			switch (currentEvent.m_eventType)
			{
				case (F_KEY_DOWN): toggleFullscreen(); printf("Handled event: %d\n", F_KEY_DOWN); break;
				case (RESIZE_WINDOW): resize(currentEvent.m_xPos, currentEvent.m_yPos); printf("Handled event: %d", RESIZE_WINDOW); break;
				default: 
				{
					refEventQ.push(currentEvent);
					++eventQCounter;
					break;
				}
			}

			doneProcessingEvents = refEventQ.empty() || eventQCounter > refEventQ.size();
		}
	}

	SDL_Window* WindowManager::getWindow() const
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

	void WindowManager::free()
	{
		SDL_DestroyWindow(window);
		window = nullptr;
	}

}