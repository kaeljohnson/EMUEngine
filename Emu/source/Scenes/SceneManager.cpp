#pragma once

#include "../../include/Scenes/SceneManager.h"
#include "../../include/Scenes/Scene.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	SceneManager::SceneManager() : m_newSceneStarting(true), m_currentScene(nullptr), m_queuedSceneName(""), m_scenes()
	{
	}

	void SceneManager::AddScene(std::string sceneName, std::shared_ptr<Scene> scene)
	{
		m_scenes[sceneName] = scene;
	}

	void SceneManager::LoadScene(std::string sceneName)
	{
		if (m_scenes.find(sceneName) != m_scenes.end())
		{
			LoadScene(m_scenes[sceneName]);
		}
		else
		{
			ENGINE_CRITICAL_D("Scene not found in SceneManager");
		}
	}

	void SceneManager::LoadScene(std::shared_ptr<Scene> scene)
	{
		if (m_currentScene) { m_currentScene->OnSceneEnd(); }
		m_currentScene = scene;
		m_currentScene->OnScenePlay();
	}

	void SceneManager::UnloadCurrentScene()
	{
		// Save what needs to be saved in current scene...
		// 
		// m_currentScene->Unload();
	}

	// Only called when the application is closing.
	void SceneManager::Cleanup()
	{
		for (auto& scene : m_scenes)
		{
			scene.second->OnSceneEnd();
		}
		m_scenes.clear();
	}
}