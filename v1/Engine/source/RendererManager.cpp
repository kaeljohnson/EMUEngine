#pragma once

#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

#include "../include/RendererManager.h"
#include "../include/Logging/Logger.h"

namespace Engine
{
	RendererManager::RendererManager(SDL_Window* window)
		: renderer(nullptr)
	{
		// Assign SDL renderer pointer to the return value of the SDL_CreateRenderer function. 
		// Note: Hover over function to understand the arguments it takes.
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // rendering driver?
		if (renderer == nullptr)
		{
			ENGINE_CRITICAL("Renderer could not be created! SDL Error: {}", SDL_GetError());
		}

		SDL_SetRenderDrawColor(renderer, 'd3', 'd3', 'd3', SDL_ALPHA_OPAQUE);
	}

	const SDL_Renderer* RendererManager::getRenderer() const
	{
		return renderer;
	}

	// Definition for loadTexture function in RendererManager. 
	// Returns a const SDL_Texture*. Takes a file (probably a sprite sheet) 
	// full of the textures we want to load.
	const SDL_Texture* RendererManager::loadTexture(const char* filePath)
	{
		SDL_Texture* texture =  IMG_LoadTexture(renderer, filePath);
		if (texture == nullptr)
		{
			ENGINE_CRITICAL("Failed to load texture. SDL Error: {}", SDL_GetError());
		}
		return texture;
	}

	// Wrapper for SDL_RenderClear. Clears the screen.
	void RendererManager::clearScreen()
	{
		SDL_RenderClear(renderer);
	}

	// Wrapper for SDL_RenderPresent. Talks to the actual hardwares renderer to display the renderer.
	void RendererManager::display()
	{
		SDL_RenderPresent(renderer);
	}

	// Definition of render function for the RendererManager class. Takes a SDL_Rect reference which will be rendered.
	void RendererManager::render(SDL_Rect& rect)
	{
		// The x, y, height, and width of the portion of the texture we want to render.
		SDL_Rect src = { 0, 0, 0, 0 };																								

		// The x and y coordinates correspond to the coordinates on the screen in pixels. 
		// The width and height are the width and height of the rect we want to render.
		SDL_Rect dst = { rect.x, rect.y, rect.w, rect.h };																			

		// Render the rect to the screen. The second argument in this function takes
		//  the texture we want to pull the src rect from. It is nullptr right now 
		// because we don't have any textures to render.
		SDL_RenderCopy(renderer, nullptr, &src, &dst);
	}
	void RendererManager::free()
	{
		// Destroy the render, which SDL uses dynamic memory to allocate.
		SDL_DestroyRenderer(renderer);

		// Now that we freed the renderer, the pointer is still attached to that memory.
		//  This is bad because we don't control what is there anymore, so it needs to be set back to nullptr.
		renderer = nullptr;																											
	}

}