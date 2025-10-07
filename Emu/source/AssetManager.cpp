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

	void AssetManager::LoadSound(Entity entity, const std::string& filePath)
	{
		// Check if already loaded
		auto it = m_soundIndices.find(filePath);
		if (it != m_soundIndices.end())
		{
			ENGINE_INFO("Adding entity to sound: " + std::to_string(entity));

			// Sound already loaded, just map entity to existing sound
			m_soundNames[entity] = filePath;
			return;
		}

		// Otherwise, load new sound
		ENGINE_INFO("Loading new sound: " + filePath);

		// Load sound
		Mix_Chunk* ptrSound = Mix_LoadWAV(filePath.c_str());
		if (!ptrSound)
		{
			ENGINE_CRITICAL("Failed to load sound: " + filePath + ", Mix_Error: " + std::string(Mix_GetError()));
			return;
		}

		// Store sound
		ENGINE_INFO("Sound loaded: " + filePath);
		m_loadedSounds.push_back(ptrSound);
		size_t index = m_loadedSounds.size() - 1;
		m_soundNames[entity] = filePath;
		m_soundIndices[filePath] = index;
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
		m_soundNames.clear();
	}

	void* AssetManager::GetSound(Entity entity)
	{
		auto filePathIterator = m_soundNames.find(entity);
		if (filePathIterator == m_soundNames.end()) return nullptr;

		auto indexIterator = m_soundIndices.find(filePathIterator->second);
		if (indexIterator == m_soundIndices.end()) return nullptr;

		return m_loadedSounds.at(indexIterator->second);
	}
}