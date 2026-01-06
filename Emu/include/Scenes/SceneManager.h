#pragma once

#include "../Includes.h"
#include "../ECS/ECS.h"
#include "Scene.h"
#include "../AssetManager.h"
#include "../Logging/Logger.h"

namespace Engine
{
	class SceneManager
	{
	public:
		void AddScene(std::string sceneName, AssetManager& refAssetManager);

		inline Scene* GetCurrentScene() const { return m_ptrCurrentScene; };

		const std::vector<Entity>& GetTileMapEntities(const std::string& sceneName, const size_t tileId) const;
		const Entity GetEntity(const std::string& sceneName, const size_t tileId);
		const std::vector<Entity>& GetEntities(const std::string& sceneName, const size_t tileId);

		void RegisterOnScenePlayEvent(const std::string& sceneName, std::function<void()>& func);
		void RegisterContactCallback(const std::string& sceneName, ContactType contactType, const size_t entityA, const size_t entityB, Scene::ContactCallback callback);
		void RegisterContactCallback(const std::string& sceneName, ContactType contactType, const size_t entity, Scene::ContactCallback callback);
		void SetGravity(const std::string& sceneName, const Math2D::Point2D<float> gravity);
		void AddTileMap(const std::string& sceneName, const std::string& mapFileName, const std::string& rulesFileName);
		void SetLevelDimensions(const std::string& sceneName, const Math2D::Point2D<int> dimensions);

		template <typename T, typename... Args>
		void AddComponent(const std::string sceneName, const size_t tileId, Args&&... componentArgs)
		{
			auto it = m_scenes.find(sceneName);
			if (it == m_scenes.end())
			{
				ENGINE_CRITICAL_D("Scene not found in SceneManager");
				return;
			}

			const std::vector<Entity>& entities = it->second.GetTileMapEntities(tileId);
			for (Entity entity : entities)
			{
				m_refECS.AddComponent<T>(entity, std::forward<Args>(componentArgs)...);
			}
		}
	public:
		SceneManager(ECS& refECS);
		~SceneManager() = default;

		void CheckForSceneChange();
		void QueueNewScene(const std::string name) { m_queuedSceneName = name; }
		void LoadQueuedScene();
		void UnloadCurrentScene();
		void Cleanup();

		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;
		SceneManager(SceneManager&&) = delete;
		SceneManager& operator=(SceneManager&&) = delete;

	private:
		ECS& m_refECS;
		std::unordered_map<std::string, Scene> m_scenes;
		std::string m_queuedSceneName;
		Scene* m_ptrCurrentScene;
	};
}