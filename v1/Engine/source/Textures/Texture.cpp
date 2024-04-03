#pragma once

#include <SDL.h>

#include "../../include/Textures/Texture.h"

#include "../../include/Application.h"

namespace Engine
{
	Texture::Texture(int r, int g, int b)
	{
		// TEMP
		Application* appInstance = Application::GetInstance();

		// Temp
		SDL_Surface* surface = SDL_CreateRGBSurface(0, 1000, 1000, 32, 0, 0, 0, 0);
		SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, r, g, b));
		m_texture = SDL_CreateTextureFromSurface(appInstance->getRenderer(), surface);
		SDL_FreeSurface(surface);
	}
}