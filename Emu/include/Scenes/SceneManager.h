#pragma once

#include "../Includes.h"
#include "../ECS/ECS.h"
#include "Scene.h"

namespace Engine
{
	class SceneManager
	{
	public:
		void AddScene(std::string sceneName, ECS& refECS);
		void LoadScene(std::string sceneName);
		void LoadScene(Scene& refScene);
		void UnloadCurrentScene();

		inline Scene* GetCurrentScene() const { return m_ptrCurrentScene; };
		inline Scene& GetScene(const std::string& sceneName) { return m_scenes.at(sceneName); }
	public:
		SceneManager();
		~SceneManager() = default;

		const bool IsNewSceneStarting() const { return m_newSceneStarting; }
		void QueueNewScene(const std::string name) { m_newSceneStarting = true; m_queuedSceneName = name; }
		void LoadQueuedScene() { LoadScene(m_queuedSceneName); }
		void NewSceneStarted() { m_newSceneStarting = false; }
		void Cleanup();

		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;
		SceneManager(SceneManager&&) = delete;
		SceneManager& operator=(SceneManager&&) = delete;

	private:
		std::unordered_map<std::string, Scene> m_scenes;
		std::string m_queuedSceneName;
		Scene* m_ptrCurrentScene;
		bool m_newSceneStarting;
	};
}