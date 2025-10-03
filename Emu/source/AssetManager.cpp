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
}