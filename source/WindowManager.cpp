#pragma once

#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>

#include "../headers/WindowManager.h"

WindowManager::WindowManager(const char* windowTitle, int pixelWidth, int pixelHeight)
	: window(nullptr)
{
	window = SDL_CreateWindow(
		windowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, pixelWidth, pixelHeight, SDL_WINDOW_SHOWN
	);
	if (window == nullptr)
	{
		printf("Window not created! SDL_Error: %s\n", SDL_GetError());
	}
}

SDL_Window* WindowManager::getWindow() const
{
	return window;
}

void WindowManager::free()
{
	SDL_DestroyWindow(window);
	window = nullptr;
}