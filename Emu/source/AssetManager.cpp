#pragma once

#include "../include/AssetManager.h"

#include "../include/ISDL/ISDL.h"

namespace Engine
{
	AssetManager::AssetManager() : m_ptrRenderer(nullptr)
	{
	}

	void AssetManager::GiveRenderer(void* ptrRenderer)
	{
		m_ptrRenderer = ptrRenderer;
	}

	void AssetManager::Shutdown()
	{
		// Clean up resources
	}

	void AssetManager::LoadTexture(Entity entity, const std::string& filePath)
	{
		// Check if already loaded
		auto it = m_textureIndices.find(filePath);
		if (it != m_textureIndices.end())
		{
			ENGINE_INFO("Adding entity to texture: " + std::to_string(entity));

			// Texture already loaded, just map entity to existing texture
			m_textureNames[entity] = filePath;
			return;
		}

		// Otherwise, load new texture
		ENGINE_INFO("Loading new texture: " + filePath);

		// Check if renderer is valid
		if (!m_ptrRenderer) 
		{
			ENGINE_CRITICAL("Renderer is not initialized.");
			return;
		}

		// Load texture
		SDLTexture* ptrTexture = IMG_LoadTexture((SDL_Renderer*)m_ptrRenderer, filePath.c_str());
		if (!ptrTexture) 
		{
			ENGINE_CRITICAL("Failed to load texture: " + filePath + ", SDL_Error: " + std::string(SDL_GetError()));
			return;
		}

		// Store texture
		ENGINE_INFO("Texture loaded: " + filePath);
		m_loadedTextures.push_back(ptrTexture);
		size_t index = m_loadedTextures.size() - 1;
		m_textureNames[entity] = filePath;
		m_textureIndices[filePath] = index;
	}

	void AssetManager::UnloadTextures()
	{
		for (void* texture : m_loadedTextures)
		{
			if (texture)
			{
				SDL_DestroyTexture((SDLTexture*)texture);
			}
		}
		m_loadedTextures.clear();
		m_textureIndices.clear();
		m_textureNames.clear();
	}

	void* AssetManager::GetTexture(Entity entity)
	{
		auto filePathIterator = m_textureNames.find(entity);
		if (filePathIterator == m_textureNames.end()) return nullptr;

		auto indexIterator = m_textureIndices.find(filePathIterator->second);
		if (indexIterator == m_textureIndices.end()) return nullptr;      

		return m_loadedTextures.at(indexIterator->second);
	}

	void AssetManager::LoadSound(const std::string& soundName, const std::string& filePath)
	{
		// Check if already loaded
		auto it = m_soundIndices.find(soundName);
		if (it != m_soundIndices.end())
		{
			ENGINE_INFO("Sound already exists! Skipping");
		}

		// Otherwise, load new sound
		ENGINE_INFO("Loading new sound: " + soundName);

		// Load sound
		Mix_Chunk* ptrSound = Mix_LoadWAV(filePath.c_str());
		if (!ptrSound)
		{
			ENGINE_CRITICAL("Failed to load sound: " + filePath + ", Mix_Error: " + std::string(Mix_GetError()));
			return;
		}

		// Store sound
		ENGINE_INFO("Sound loaded: " + soundName);
		m_loadedSounds.push_back(ptrSound);
		size_t index = m_loadedSounds.size() - 1;
		m_soundIndices[soundName] = index;
	}

	void AssetManager::UnloadSounds()
	{
		for (void* sound : m_loadedSounds)
		{
			if (sound)
			{
				Mix_FreeChunk((Mix_Chunk*)sound);
			}
		}
		m_loadedSounds.clear();
		m_soundIndices.clear();
	}

	void* AssetManager::GetSound(const std::string& soundName)
	{
		auto soundIndexIterator = m_soundIndices.find(soundName);
		if (soundIndexIterator == m_soundIndices.end()) return nullptr;

		return m_loadedSounds.at(soundIndexIterator->second);
	}
}