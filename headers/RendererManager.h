#pragma once

#include <SDL.h>													// External includes.
#include <SDL_image.h>

class RendererManager												// RendererManager class decleration.
{
private:
	SDL_Renderer* renderer = nullptr;								// Declare and set SDL_Renderer* to nullptr
public:																// Public declerations below.
	RendererManager(SDL_Window* window);							// Declare RendererManager constructor.
	const SDL_Renderer* getRenderer() const;						// Declare getRenderer function.
	const SDL_Texture* loadTexture(const char* filePath);			// Declare loadTexture function.
	void clearScreen();												// Declare clearScreen function.
	void render(SDL_Rect& rect);									// Declare render function.
	void display();													// Declare display function.
	void free();													// Declare free function.
};