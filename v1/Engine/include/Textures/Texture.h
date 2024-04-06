#pragma once

namespace Engine
{
	class Texture
	{
	public:
		SDL_Texture* m_texture;

		Texture(int r, int g, int b);
		~Texture();
	};
}