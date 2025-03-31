#pragma once

#include "../../include/Scenes/SceneManager.h"
#include "../../include/Scenes/Scene.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	SceneManager::SceneManager() : m_newSceneStarting(true), m_ptrCurrentScene(nullptr), m_queuedSceneName(""), m_scenes()
	{
		m_scenes.reserve(200);
	}

	void SceneManager::AddScene(std::string sceneName, ECS& refECS)
	{
		// m_scenes.emplace(sceneName, Scene(refECS));
		m_scenes.emplace(std::piecewise_construct,
			std::forward_as_tuple(sceneName),
			std::forward_as_tuple(refECS)); // Pass constructor args


	}

	void SceneManager::LoadScene(std::string sceneName)
	{
		if (m_scenes.find(sceneName) != m_scenes.end())
		{
			LoadScene(m_scenes.at(sceneName));
		}
		else
		{
			ENGINE_CRITICAL_D("Scene not found in SceneManager");
		}
	}

	void SceneManager::LoadScene(Scene& refScene)
	{
		if (m_ptrCurrentScene) { m_ptrCurrentScene->OnSceneEnd(); }
		m_ptrCurrentScene = &refScene;
		m_ptrCurrentScene->OnScenePlay();
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
			scene.second.OnSceneEnd();
		}
		m_scenes.clear();
	}
}