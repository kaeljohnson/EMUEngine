#pragma once

#include <vector>

#include "Scene.h"

namespace Engine
{
	class SceneManager
	{
	public:
		EMU_API void SetScene(std::shared_ptr<Scene> scene);
		EMU_API void AddScene(std::shared_ptr<Scene> scene);
		EMU_API inline std::shared_ptr<Scene> GetCurrentScene() const { return m_currentScene; };

	public:
		SceneManager();
		~SceneManager() = default;

		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;
		SceneManager(SceneManager&&) = delete;
		SceneManager& operator=(SceneManager&&) = delete;

	private:
		std::vector<std::shared_ptr<Scene>> m_scenes;

		std::shared_ptr<Scene> m_currentScene;
	};
}