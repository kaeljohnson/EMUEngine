#pragma once
#include "../Core.h"

namespace Engine
{
	class EMU_API Texture
	{
	public:
		SDL_Texture* m_texture;

		Texture(int r, int g, int b);
		~Texture();
	};
}