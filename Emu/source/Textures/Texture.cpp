#pragma once

#include "../../include/SDLWrapper/SDLWrapper.h"
#include "../../include/Textures/Texture.h"
#include "../../include/Rendering/RendererManager.h"
#include "../../include/Rendering/Renderer.h"

#include "../../include/Logging/Logger.h"

namespace Engine
{
	Texture::Texture(int r, int g, int b)
	{
		// Temp
		SDLSurface* surface = SDL_CREATE_RGB_SURFACE(0, 1000, 1000, 32, 0, 0, 0, 0);
		SDL_FILL_RECT(surface, NULL, SDL_MAP_RGB(surface->format, r, g, b));
		m_texture = SDL_CREATE_TEXTURE_FROM_SURFACE(static_pointer_cast<Renderer>(RendererManager::GetInstance()->GetRenderer())->renderer, surface);
		SDL_FREE_SURFACE(surface);
	}

	/*Texture::Texture(const char* filePath)
	{
		m_texture = IMG_LoadTexture(static_pointer_cast<Renderer>(RendererManager::GetInstance()->GetRenderer())->renderer, filePath);
		if (m_texture == nullptr)
		{
			ENGINE_CRITICAL("Failed to load texture. SDL Error: {}", SDL_GET_ERROR());
		}
	}*/

	Texture::~Texture() 
	{
		SDL_DESTROY_TEXTURE(m_texture);
	}
}