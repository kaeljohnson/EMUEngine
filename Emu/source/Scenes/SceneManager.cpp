#pragma once

#include "../../include/Scenes/SceneManager.h"

namespace Engine
{
	SceneManager::SceneManager() 
	{
		
	}

	void SceneManager::SetScene(std::shared_ptr<Scene> scene)
	{
		m_currentScene = scene;
	}

	void SceneManager::AddScene(std::shared_ptr<Scene> scene)
	{
		m_scenes.push_back(scene);
	}
}