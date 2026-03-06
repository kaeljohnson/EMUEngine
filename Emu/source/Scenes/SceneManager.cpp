#pragma once

#include "../../include/Scenes/SceneManager.h"
#include "../../include/Scenes/Scene.h"
#include "../../Public/Logger.h"
#include "../../include/Events/IOEventSystem.h"

namespace Engine
{
	SceneManager::SceneManager(ECS& refECS) : m_refECS(refECS), m_ptrCurrentScene(nullptr), m_queuedSceneName(""), m_scenes()
	{
		m_scenes.reserve(200);
	}

	void SceneManager::AddScene(std::string sceneName, AssetManager& refAssetManager, IOEventSystem& refIOEventSystem)
	{
		m_scenes.emplace(std::piecewise_construct,
			std::forward_as_tuple(sceneName),
			std::forward_as_tuple(m_refECS, refAssetManager, refIOEventSystem));
	}

	bool SceneManager::IsSceneChanging()
	{
		if (!m_queuedSceneName.empty())
		{
			UnloadCurrentScene();
			LoadQueuedScene();
			m_queuedSceneName.clear();
			return true;
		}

		return false;
	}

	void SceneManager::LoadQueuedScene()
	{
		auto it = m_scenes.find(m_queuedSceneName);
		if (it != m_scenes.end())
		{	
			m_ptrCurrentScene = &it->second;
			m_ptrCurrentScene->OnScenePlay();
		}
		else
		{
			ENGINE_CRITICAL("Scene not found in SceneManager: {}", m_queuedSceneName);
		}
	}

	const Entity SceneManager::GetEntity(const std::string& sceneName, const size_t tileId)
	{
		auto it = m_scenes.find(sceneName);
		if (it != m_scenes.end())
		{
			return it->second.GetTileMapEntity(tileId);
		}
		else
		{
			ENGINE_CRITICAL("Scene not found in SceneManager: {}", sceneName);
			return m_refECS.INVALID_ENTITY;
		}
	}

	const std::vector<Entity>& SceneManager::GetEntities(const std::string& sceneName, const size_t tileId)
	{
		auto it = m_scenes.find(sceneName);
		if (it != m_scenes.end())
		{
			return it->second.GetTileMapEntities(tileId);
		}
		else
		{
			ENGINE_CRITICAL("Scene not found in SceneManager: {}", sceneName);
			static const std::vector<Entity> emptyVector;
			return emptyVector;
		}
	}

	void SceneManager::UnloadCurrentScene()
	{
		// Save what needs to be saved in current scene...
		// 
		if (m_ptrCurrentScene) { m_ptrCurrentScene->OnSceneEnd(); }
		m_ptrCurrentScene = nullptr;
	}

	// Only called when the application is closing.
	void SceneManager::Cleanup()
	{
		for (auto& scene : m_scenes)
		{
			scene.second.OnSceneEnd();
		}
		m_scenes.clear();
	}

	const std::vector<Entity>& SceneManager::GetTileMapEntities(const std::string& sceneName, const size_t tileId) const 
	{
		auto it = m_scenes.find(sceneName);
		if (it != m_scenes.end()) 
		{
			return it->second.GetTileMapEntities(tileId);
		}
		else 
		{
			static const std::vector<Entity> emptyVector;
			return emptyVector;
		}
	}

	void SceneManager::RegisterOnScenePlayEvent(const std::string& sceneName, std::function<void()>& func)
	{
		auto it = m_scenes.find(sceneName);
		if (it != m_scenes.end())
		{
			it->second.RegisterOnScenePlayEvent(func);
		}
		else
		{
			ENGINE_CRITICAL("Scene not found in SceneManager: {}", sceneName);
		}

	}

	void SceneManager::RegisterOnSceneEndEvent(const std::string& name, std::function<void()> func)
	{
		auto it = m_scenes.find(name);
		if (it != m_scenes.end())
		{
			it->second.RegisterOnSceneEndEvent(func);
		}
		else
		{
			ENGINE_CRITICAL("Scene not found in SceneManager: {}", name);
		}
	}

	void SceneManager::RegisterContactCallback(const std::string& sceneName, ContactType contactType, const size_t entityA, const size_t entityB, Scene::ContactCallback callback)
	{
		auto it = m_scenes.find(sceneName);
		if (it != m_scenes.end())
		{
			it->second.RegisterContactCallback(contactType, entityA, entityB, callback);
		}
		else
		{
			ENGINE_CRITICAL("Scene not found in SceneManager: {}", sceneName);

		}
	}

	void SceneManager::RegisterContactCallback(const std::string& sceneName, ContactType contactType, const size_t entity, Scene::ContactCallback callback)
	{
		auto it = m_scenes.find(sceneName);
		if (it != m_scenes.end())
		{
			it->second.RegisterContactCallback(contactType, entity, callback);
		}
		else
		{
			ENGINE_CRITICAL("Scene not found in SceneManager: {}", sceneName);

		}
	}

	void SceneManager::AddIOEvent(const std::string& sceneName, IOEventType type)
	{
		auto it = m_scenes.find(sceneName);
		if (it != m_scenes.end())
		{
			it->second.AddIOEvent(type);
		}
		else
		{
			ENGINE_CRITICAL("Scene not found in SceneManager: {}", sceneName);

		}
	}

	void SceneManager::RemoveIOEvent(const std::string& sceneName, IOEventType type)
	{
		auto it = m_scenes.find(sceneName);
		if (it != m_scenes.end())
		{
			it->second.RemoveIOEvent(type);
		}
		else
		{
			ENGINE_CRITICAL("Scene not found in SceneManager: {}", sceneName);

		}
	}

	void SceneManager::SetGravity(const std::string& sceneName, const Math2D::Point2D<float> gravity)
	{
		auto it = m_scenes.find(sceneName);
		if (it != m_scenes.end())
		{
			it->second.SetGravity(gravity);
		}
		else
		{
			ENGINE_CRITICAL("Scene not found in SceneManager: {}", sceneName);
		}
	}

	void SceneManager::AddTileMap(const std::string& sceneName, const std::string& mapFileName, const std::string& rulesFileName)
	{
		auto it = m_scenes.find(sceneName);
		if (it != m_scenes.end())
		{
			it->second.AddTileMap(mapFileName, rulesFileName);
		}
		else
		{
			ENGINE_CRITICAL("Scene not found in SceneManager: {}", sceneName);
		}
	}

	void SceneManager::SetLevelDimensions(const std::string& sceneName, const Math2D::Point2D<int> dimensions)
	{
		auto it = m_scenes.find(sceneName);
		if (it != m_scenes.end())
		{
			it->second.SetLevelDimensions(dimensions);
		}
		else
		{
			ENGINE_CRITICAL("Scene not found in SceneManager: {}", sceneName);
		}
	}

	void SceneManager::AddLayer(std::string& sceneName, int layerId, int layerIdx, float parallaxFactor, bool hasPhysics)
	{
		auto it = m_scenes.find(sceneName);
		if (it != m_scenes.end())
		{
			it->second.AddLayer(sceneName, layerId, parallaxFactor, hasPhysics);
		}
		else
		{
			ENGINE_CRITICAL("Scene not found in SceneManager: {}", sceneName);
		}
	}
}