#pragma once
#include "../Core.h"

namespace Engine
{

	// Temporary class for creating textures.

	class EMU_API Texture
	{
	public:
		SDL_Texture* m_texture;

		Texture(int r, int g, int b);
		~Texture();
	};
}