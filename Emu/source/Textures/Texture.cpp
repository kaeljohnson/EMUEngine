#pragma once

#include "../../include/SDLWrapper/SDLWrapper.h"
#include "../../include/Textures/Texture.h"
#include "../../include/RendererManager.h"
#include "../../include/CallbackSystem/CallbackSystem.h"

namespace Engine
{
	Texture::Texture(int r, int g, int b)
	{
		// Temp
		SDLSurface* surface = SDL_CREATE_RGB_SURFACE(0, 1000, 1000, 32, 0, 0, 0, 0);
		SDL_FILL_RECT(surface, NULL, SDL_MAP_RGB(surface->format, r, g, b));
		m_texture = SDL_CREATE_TEXTURE_FROM_SURFACE(RendererManager::GetInstance()->GetRenderer(), surface);
		SDL_FREE_SURFACE(surface);
	}

	Texture::~Texture() 
	{
		SDL_DESTROY_TEXTURE(m_texture);
	}
}