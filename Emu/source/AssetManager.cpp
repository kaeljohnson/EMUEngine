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
		auto it = m_textures.find(entity);
		if (it != m_textures.end())
			ENGINE_INFO("Overwriting texture for entity " + std::to_string(entity));

		// Check if renderer is valid
		if (!m_ptrRenderer) {
			ENGINE_CRITICAL("Renderer is not initialized.");
			return;
		}

		// Load texture
		SDLTexture* ptrTexture = IMG_LoadTexture((SDL_Renderer*)m_ptrRenderer, filePath.c_str());
		if (!ptrTexture) {
			ENGINE_CRITICAL("Failed to load texture: " + filePath + ", SDL_Error: " + std::string(SDL_GetError()));
			return;
		}

		// Store texture and return new ID
		m_textures[entity] = ptrTexture;
	}

	void* AssetManager::GetTexture(Entity entity)
	{
		if (m_textures.find(entity) != m_textures.end())
			return m_textures[entity];
		else
			return nullptr;
	}
}