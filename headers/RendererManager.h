#pragma once

#include <SDL.h>
#include <SDL_image.h>

class RendererManager
{
private:
	SDL_Renderer* renderer = nullptr;
public:
	RendererManager(SDL_Window* window);
	const SDL_Renderer* getRenderer() const;
	const SDL_Texture* loadTexture(const char* filePath);
	void clearScreen();
	void render(SDL_Rect& rect);
	void display();
	void free();
};