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
		void UnloadTextures();
		void* GetTexture(Entity id);

		void PrepareSoundStorage(const size_t numSounds);
		void LoadSound(int soundIndex, const std::string& filePath);
		void UnloadSounds();
		void* GetSound(int soundName);

	private:
		void* m_ptrRenderer; // Pointer to the SDL_Renderer.

		std::vector<void*> m_loadedTextures;						// Store loaded textures
		std::unordered_map<std::string, size_t> m_textureIndices;	// file path to index in m_loadedTextures.
		std::unordered_map<Entity, std::string> m_textureNames;		// index in m_loadedTextures to entities using it.

		std::vector<void*> m_loadedSounds;							// Store loaded sounds
	};
}