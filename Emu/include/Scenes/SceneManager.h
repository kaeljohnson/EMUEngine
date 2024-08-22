#pragma once

#include <vector>

#include "Scene.h"

namespace Engine
{
	class SceneManager
	{
	public:
		EMU_API void AddScene(std::shared_ptr<Scene> scene);
		EMU_API void LoadScene(std::string sceneName);
		EMU_API void LoadScene(std::shared_ptr<Scene> scene);
		EMU_API void UnloadCurrentScene();

		EMU_API inline std::shared_ptr<Scene> GetCurrentScene() const { return m_currentScene; };
	public:
		SceneManager();
		~SceneManager() = default;

		const bool IsNewSceneStarting() const { return m_newSceneStarting; }
		void NewSceneStarted() { m_newSceneStarting = false; }

		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;
		SceneManager(SceneManager&&) = delete;
		SceneManager& operator=(SceneManager&&) = delete;

	private:
		std::vector<std::shared_ptr<Scene>> m_scenes;

		std::shared_ptr<Scene> m_currentScene;

		bool m_newSceneStarting;
	};
}