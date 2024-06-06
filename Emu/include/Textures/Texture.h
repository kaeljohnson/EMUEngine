#pragma once

#include "ITexture.h"
#include "../SDLWrapper/SDLWrapper.h"

namespace Engine
{
	// Temporary class for creating textures.

	class Texture: public ITexture
	{
	public:
		SDLTexture* m_texture;

		Texture(int r, int g, int b);
		// Texture(const char* filePath);

		~Texture();
	};
}