#pragma once

#include "../Public/Includes.h"

#include "ECS/ECS.h"

namespace Engine
{
	/**
	* @class AssetManager
	* 
	* @brief Manages loading and unloading of assets such as textures and sounds.
	*/
	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager();

		/**
		* @brief Lends the renderer to the AssetManager for texture loading.
		*
		* @param ptrRenderer Pointer to the SDL_Renderer.
		*/
		bool GiveRenderer(void* ptrRenderer);

		/**
		* @brief Shutdown asset manager, freeing all loaded assets.
		*/
		void Shutdown();

		/**
		* @brief Loads texture by calling IMG_LoadTexture. Adds loaded texture to m_loadedTextures.
		* 
		* @param entity The entity to associate the texture with.
		* @param filePath The file path of the texture to load.
		* 
		* @return Pointer to the loaded texture.
		*/
		void* LoadTexture(Entity entity, const std::string& filePath);

		/**
		* @brief Unloads all loaded textures.
		*/
		void UnloadTextures();

		/**
		* @brief Gets the texture pointer associated with the given entity.
		* 
		* @param entity The entity whose texture to get.
		* 
		* @return Pointer to the texture.
		*/
		void* GetTexture(Entity entity);

		/**
		* @brief Prepares storage for a given number of sounds.
		* 
		* @param numSounds The number of sounds to prepare storage for.
		*/
		void PrepareSoundStorage(const size_t numSounds);

		/**
		* @brief Loads a sound from file and stores it at the given index.
		*	
		* @param soundIndex The index to store the sound at.
		* @param filePath The file path of the sound to load.
		*/
		void LoadSound(int soundIndex, const std::string& filePath);

		/**
		* @brief Unloads all loaded sounds.
		*/
		void UnloadSounds();

		/**
		* @brief Gets the sound pointer at the given index.
		* 
		* @param soundName The index of the sound to get.
		* 
		* @return Pointer to the sound.
		*/
		void* GetSound(int soundName);

	private:
		void* m_ptrRenderer; /// Pointer to the SDL_Renderer.

		std::vector<void*> m_loadedTextures;						/// Store loaded textures
		std::unordered_map<std::string, size_t> m_textureIndices;	/// file path to index in m_loadedTextures.
		std::unordered_map<Entity, std::string> m_textureNames;		/// index in m_loadedTextures to entities using it.

		std::vector<void*> m_loadedSounds;							/// Store loaded sounds
	};
}