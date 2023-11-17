#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class WindowManager
{
private:
	SDL_Window* window = nullptr;

public:
	WindowManager(const char* windowTitle, const int pixelWidth, const int pixelHeight);
	SDL_Window* getWindow() const;
	void free();
};