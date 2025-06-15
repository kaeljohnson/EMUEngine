#pragma once

#include "Includes.h"

#include "ECS/ECS.h"

namespace Engine
{
	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager() = default;

		void GiveRenderer(void* ptrRenderer);
		void Shutdown();

		void LoadTexture(Entity entity, const std::string& filePath);
		void* GetTexture(Entity id);

	private:
		void* m_ptrRenderer; // Pointer to the SDL_Renderer.

		// Temp: Replace with more robust/performant container later.
		std::unordered_map<size_t, void*> m_textures; // void* used to hide SDL_Texture type from client.
	};
}