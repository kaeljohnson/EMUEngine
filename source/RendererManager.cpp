#pragma once

#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

#include "../headers/RendererManager.h"

RendererManager::RendererManager(SDL_Window* window)
	: renderer(nullptr)
{
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
	}

	SDL_SetRenderDrawColor(renderer, 'd3', 'd3', 'd3', SDL_ALPHA_OPAQUE);
}

const SDL_Renderer* RendererManager::getRenderer() const
{
	return renderer;
}

const SDL_Texture* RendererManager::loadTexture(const char* filePath)
{
	SDL_Texture* texture = nullptr;
	texture = IMG_LoadTexture(renderer, filePath);
	if (texture == nullptr)
	{
		printf("Failed to load texture. SDL Error: %s\n", SDL_GetError());
	}
	return texture;
}

void RendererManager::clearScreen()
{
	SDL_RenderClear(renderer);
}

void RendererManager::display()
{
	SDL_RenderPresent(renderer);
}

void RendererManager::render(SDL_Rect& rect)
{
	SDL_Rect src = { 0, 0, 0, 0 };

	SDL_Rect dst = { rect.x, rect.y, rect.w, rect.h };

	SDL_RenderCopy(renderer, nullptr, &src, &dst);
}
void RendererManager::free()
{
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
}