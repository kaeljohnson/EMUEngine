#pragma once

#include "../../Public/Includes.h"
#include "../ECS/ECS.h"
#include "Scene.h"
#include "../AssetManager.h"
#include "../../Public/Logger.h"
#include "../Events/IOEventSystem.h"

namespace Engine
{
	/**
	* @class SceneManager
	* 
	* @brief Manages all the scenes in the application. Controls when to start and stop scenes. Loads and unloads scenes as needed.
	* Manages how scenes are stored.
	*/
	class SceneManager
	{
	public:

		/**
		* @brief Adds a new scene to the scene manager.
		* 
		* @param sceneName The name of the scene to add.
		* @param refAssetManager Reference to the AssetManager instance.
		* @param refIOEventSystem Reference to the IOEventSystem instance.
		*/
		void AddScene(std::string sceneName, const std::string& rulesFileName, AssetManager& refAssetManager, IOEventSystem& refIOEventSystem);

		/**
		* @brief Gets a pointer to the current active scene.
		* 
		* @return Pointer to the current active scene.
		*/
		inline Scene* GetCurrentScene() const { return m_ptrCurrentScene; };

		/**
		* @brief Gets the entities associated with a specific tile ID in a given scene.
		* 
		* @param sceneName The name of the scene.
		* @param tileId The tile ID to get entities for.
		* 
		* @return A vector of entities associated with the specified tile ID.
		*/
		const std::vector<Entity>& GetTileMapEntities(const std::string& sceneName, const int layer, const size_t tileId) const;

		/**
		* @brief Gets the entity associated with a specific tile ID in a given scene.
		* 
		* @param sceneName The name of the scene.
		* @param tileId The tile ID to get the entity for.
		* 
		* @return The entity associated with the specified tile ID.
		*/
		const Entity GetEntity(const std::string& sceneName, const int layer, const size_t tileId);

		/**
		* @brief Gets the entities associated with a specific tile ID in a given scene.
		* 
		* @param sceneName The name of the scene.
		* @param tileId The tile ID to get entities for.
		* 
		* @return A vector of entities associated with the specified tile ID.
		*/
		const std::vector<Entity>& GetEntities(const std::string& sceneName, const int layer, const size_t tileId);

		/**
		* @brief Registers a function to be called when the scene starts playing.
		* 
		* @param sceneName The name of the scene.
		* @param func The function to register.
		*/
		void RegisterOnScenePlayEvent(const std::string& sceneName, std::function<void()>& func);

		/**
		* @brief Registers a function to be called when the scene ends.
		* 
		* @param name The name of the scene.
		* @param func The function to register.
		*/
		void RegisterOnSceneEndEvent(const std::string& name, std::function<void()> func);

		/**
		* @brief Registers a contact callback for specific entities in a given scene.
		* 
		* @param sceneName The name of the scene.
		* @param contactType The type of contact event.
		* @param entityA The first entity involved in the contact.
		* @param entityB The second entity involved in the contact.
		* @param callback The callback function to be invoked when the contact event occurs.
		*/
		void RegisterContactCallback(const std::string& sceneName, ContactType contactType, const Math2D::Point2D<size_t> tileIdA, const Math2D::Point2D<size_t> tileIdB, Scene::ContactCallback callback);

		/**
		* @brief Registers a contact callback for a specific entity in a given scene.
		* 
		* @param sceneName The name of the scene.
		* @param contactType The type of contact event.
		* @param entity The entity involved in the contact.
		* @param callback The callback function to be invoked when the contact event occurs.
		*/
		void RegisterContactCallback(const std::string& sceneName, ContactType contactType, const Math2D::Point2D<size_t> tileId, Scene::ContactCallback callback);

		/**
		* @brief Sets the gravity for the physics simulation in a given scene.
		* 
		* @param sceneName The name of the scene.
		* @param gravity A Math2D::Point2D<float> representing the gravity vector.
		*/
		void SetGravity(const std::string& sceneName, const Math2D::Point2D<float> gravity);

		/**
		* @brief Adds a component to all entities associated with a specific tile ID in a given scene.
		* 
		* @tparam T The type of component to add.
		* @param sceneName The name of the scene.
		* @param tileId The tile ID to add the component to.
		* @param componentArgs The arguments to pass to the component constructor.
		*/
		template <typename T, typename... Args>
		void AddComponent(const std::string& sceneName, const int layer, size_t tileId, Args&&... componentArgs)
		{
			auto it = m_scenes.find(sceneName);
			if (it == m_scenes.end())
			{
				ENGINE_CRITICAL_D("Scene not found in SceneManager: {}", sceneName);
				return;
			}

			const auto& entities = it->second.GetTileMapEntities(layer, tileId);

			auto stored = std::make_tuple(std::forward<Args>(componentArgs)...);

			for (Entity entity : entities)
			{

				ENGINE_CRITICAL_D("TILE ID: {}, ENTITY: {}", tileId, entity);
				std::apply([&](auto&... xs) {
					m_refECS.AddComponent<T>(entity, xs...);
					}, stored);
			}
		}


		void AddIOEvent(const std::string& sceneName, IOEventType type);
		void RemoveIOEvent(const std::string& sceneName, IOEventType type);

	public:
		/**
		* @brief Constructor
		* 
		* @param refECS Reference to the ECS instance.
		*/
		SceneManager(ECS& refECS);
		~SceneManager() = default;


		/**
		* @brief Checks if a scene change has been queued and performs the change if necessary.
		* 
		* @note Returning a bool in case client needs to do additional cleanup that the scene is not responsible in between runtime contexts.
		* 
		* @return a bool indicating if a new scene is starting. 
		*/
		bool IsSceneChanging();

		/**
		* @brief Queues a new scene to be loaded. Application checks if a new scene is queued at the start of each frame.
		* 
		* @param name The name of the scene to queue.
		*/
		void QueueNewScene(const std::string name) { m_queuedSceneName = name; }

		/**
		* @brief Loads the queued scene, unloading the current scene in the process. 
		* This includes loading all entities and components for the new scene. 
		* This also loads all assets required for the new scene, and calls the on scene play events.
		*/
		void LoadQueuedScene();

		/**
		* @brief Unloads the current active scene. Unloads all entities and components associated with the scene,
		* along with the assets loaded for the scene.
		*/
		void UnloadCurrentScene();

		/**
		* @brief Cleans up all scenes managed by the SceneManager. Calls OnSceneEnd for each scene and clears the scene storage.
		*/
		void Cleanup();

		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;
		SceneManager(SceneManager&&) = delete;
		SceneManager& operator=(SceneManager&&) = delete;

	private:
		ECS& m_refECS;										/// Reference to the ECS instance.
		std::unordered_map<std::string, Scene> m_scenes;	/// Storage for all scenes managed by the SceneManager, mapped by scene name.
		std::string m_queuedSceneName;						/// Name of the scene that is queued to be loaded.
		Scene* m_ptrCurrentScene;							/// Pointer to the current active scene.
	};
}