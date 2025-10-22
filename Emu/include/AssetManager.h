#pragma once

#include "Includes.h"

#include "ECS/ECS.h"

namespace Engine
{
	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager();

		/*
			Lends the renderer to the AssetManager for texture loading.
			arg1: ptrRenderer: Pointer to the SDL_Renderer.
		*/
		void GiveRenderer(void* ptrRenderer);

		/*
			Shutdown asset manager, freeing all loaded assets.
		*/
		void Shutdown();

		/*
			Loads texture by calling IMG_LoadTexture. Adds loaded texture to m_loadedTextures.
			arg1: entity: The entity to associate the texture with.
		*/
		void LoadTexture(Entity entity, const std::string& filePath);

		/*
			Unloads all loaded textures.
		*/
		void UnloadTextures();

		/*
			Gets the texture pointer associated with the given entity.
			arg1: id: The entity whose texture to get.
			returns: Pointer to the texture.
		*/
		void* GetTexture(Entity id);

		/*
			Prepares storage for a given number of sounds.
			arg1: numSounds: The number of sounds to prepare storage for.
		*/
		void PrepareSoundStorage(const size_t numSounds);

		/*
			Loads a sound from file and stores it at the given index.
			arg1: soundIndex: The index to store the sound at.
			arg2: filePath: The file path of the sound to load.
		*/
		void LoadSound(int soundIndex, const std::string& filePath);

		/*
			Unloads all loaded sounds.
		*/
		void UnloadSounds();

		/*
			Gets the sound pointer at the given index.
			arg1: soundName: The index of the sound to get.
			returns: Pointer to the sound.
		*/
		void* GetSound(int soundName);

	private:
		void* m_ptrRenderer; // Pointer to the SDL_Renderer.

		std::vector<void*> m_loadedTextures;						// Store loaded textures
		std::unordered_map<std::string, size_t> m_textureIndices;	// file path to index in m_loadedTextures.
		std::unordered_map<Entity, std::string> m_textureNames;		// index in m_loadedTextures to entities using it.

		std::vector<void*> m_loadedSounds;							// Store loaded sounds
	};
}