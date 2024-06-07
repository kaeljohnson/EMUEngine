#pragma once
#include "../Core.h"

#include "ITexture.h"
#include "../SDLWrapper/SDLWrapper.h"

namespace Engine
{
	// Temporary class for creating textures.

	class Texture : public ITexture
	{
	public:
		SDL_Texture* m_texture;

		Texture(int r, int g, int b);
		~Texture();
	};
}