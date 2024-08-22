#pragma once

#include "../../include/Scenes/SceneManager.h"

namespace Engine
{
	SceneManager::SceneManager() : m_newSceneStarting(false), m_currentScene(nullptr)
	{
	}

	void SceneManager::AddScene(std::shared_ptr<Scene> scene)
	{
		m_scenes.push_back(scene);
	}

	void SceneManager::LoadScene(std::string sceneName)
	{
		/*for (auto scene : m_scenes)
		{
			if (scene->GetName() == sceneName)
			{
				m_currentScene = scene;
				m_currentScene->Load();
			}
		}*/
	}

	void SceneManager::LoadScene(std::shared_ptr<Scene> scene)
	{
		if (m_currentScene) m_currentScene->OnSceneEnd();
		m_currentScene = scene;
		m_currentScene->OnScenePlay();
		m_newSceneStarting = true;
	}

	void SceneManager::UnloadCurrentScene()
	{
		// Save what needs to be saved in current scene...
		// 
		// m_currentScene->Unload();
	}
}