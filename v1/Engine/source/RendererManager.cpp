#pragma once

#include <stdio.h>																												////////////
#include <SDL.h>																												// Include external libraries.
#include <SDL_image.h>																											////////////

#include "../include/RendererManager.h"																							// Include header files.

namespace Engine
{

	RendererManager::RendererManager(SDL_Window* window)																			// Rendere manager constructor.
		: renderer(nullptr)																											// Initialization list.
	{
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // rendering driver?		// Assign SDL renderer pointer to the return value of the SDL_CreateRenderer function. Note: Hover over function to understand the arguments it takes.
		if (renderer == nullptr)																									// If the renderer is still nullptr, that means the call to SDL_CreateRenderer function failed.
		{
			printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());												// Print the last SDL error.
		}

		SDL_SetRenderDrawColor(renderer, 'd3', 'd3', 'd3', SDL_ALPHA_OPAQUE);														// Sets the background color. 'd3' means gray. We will have a background texture eventually.
	}

	const SDL_Renderer* RendererManager::getRenderer() const																		// Getter for getting the renderer. Reason we have gettters instead of letting client code access the renderer directly is so we can ensure it is protectd, i.e. will not be changed by the client.
	{																																// Note: the const-ness of getters. const at the beginning means the variable being returned from the function is const.
		return renderer;																											//									const at the end means the data members will not be changed AND that the function can be called on a const object.
	}

	const SDL_Texture* RendererManager::loadTexture(const char* filePath)															// Definition for loadTexture function in RendererManager. Returns a const SDL_Texture*. Takes a file (probably a sprite sheet) full of the textures we want to load.
	{
		SDL_Texture* texture = nullptr;																								// Create a tempary SDL_Texture* to store texture in 
		texture = IMG_LoadTexture(renderer, filePath);																				// Call to IMG_LoadTexture to store newly created texture in texture.
		if (texture == nullptr)																										// If texture is still a nullptr, print the latest SDL error.
		{
			printf("Failed to load texture. SDL Error: %s\n", SDL_GetError());
		}
		return texture;																												// return texture.
	}

	void RendererManager::clearScreen()																								// Definition for clear screen function.
	{
		SDL_RenderClear(renderer);																									// Call to SDL_RenderClear. Talks to the actual hardwares renderer to clear the screen.
	}

	void RendererManager::display()																									// Definition for display function in RendererManager.
	{
		SDL_RenderPresent(renderer);																								// Call to SDL_RenderPresent. Talks to the actual hardwares renderer to display the renderer.
	}

	void RendererManager::render(SDL_Rect& rect)																					// Definition of render function for the RendererManager class. Takes a SDL_Rect reference which will be rendered.
	{
		SDL_Rect src = { 0, 0, 0, 0 };																								// The x, y, height, and width of the portion of the texture we want to render.

		SDL_Rect dst = { rect.x, rect.y, rect.w, rect.h };																			// The x and y coordinates correspond to the coordinates on the screen in pixels. The width and height are the width and height of the rect we want to render.

		SDL_RenderCopy(renderer, nullptr, &src, &dst);																				// Render the rect to the screen. The second argument in this function takes the texture we want to pull the src rect from. It is nullptr right now because we don't have any textures to render.
	}
	void RendererManager::free()																									// Definition for the free function in the RendererManager. 
	{
		SDL_DestroyRenderer(renderer);																								// Destroy the render, which SDL uses dynamic memory to allocate.
		renderer = nullptr;																											// Now that we freed the renderer, the pointer is still attached to that memory. This is bad because we don't control what is there anymore, so it needs to be set back to nullptr.
	}

}