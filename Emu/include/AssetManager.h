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

		void LoadSound(Entity entity, const std::string& filePath);
		void UnloadSounds();
		void* GetSound(Entity id);

	private:
		void* m_ptrRenderer; // Pointer to the SDL_Renderer.

		std::vector<void*> m_loadedTextures;						// Store loaded textures
		std::unordered_map<std::string, size_t> m_textureIndices;	// file path to index in m_loadedTextures.
		std::unordered_map<Entity, std::string> m_textureNames;		// index in m_loadedTextures to entities using it.


		std::vector<void*> m_loadedSounds;							// Store loaded sounds
		std::unordered_map<std::string, size_t> m_soundIndices;		// file path to index in m_loadedSounds.
		std::unordered_map<Entity, std::string> m_soundNames;		// index in m_loadedSounds to entities using it.
	};
}