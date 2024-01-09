#pragma once

#include <stdio.h>																										// External library includes.
#include <SDL.h>																										//
#include <SDL_image.h>																									//
#include <string>																										//

#include "../include/WindowManager.h"																					// Include header files.

namespace Engine
{

	WindowManager::WindowManager(const char* windowTitle, int pixelWidth, int pixelHeight)									// Definition of WindowManager constructor.
		: window(nullptr)																									// Initialization list for WindowManager constructor.
	{
		window = SDL_CreateWindow(																							// Call to SDL_CreateWindow function. Hover over function to understand arguments.
			windowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, pixelWidth, pixelHeight, SDL_WINDOW_SHOWN
		);
		if (window == nullptr)																								// If the window is still a nullptr, print an error.
		{
			printf("Window not created! SDL_Error: %s\n", SDL_GetError());
		}
	}

	SDL_Window* WindowManager::getWindow() const																			// Getter for window stored in WindowManager.
	{
		return window;
	}

	void WindowManager::free()																								// Definition for free function in WindowManager.
	{
		SDL_DestroyWindow(window);																							// Free dynamically allocated window.
		window = nullptr;																									// Make sure window is not still attached to the freed memory.
	}

}