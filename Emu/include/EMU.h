#pragma once

#include "Core.h"
#include "Application.h"
#include "Scenes/SceneManager.h"
#include "Events/IOEventSystem.h"
#include "Physics/Physics.h"
#include "Camera/CameraInterface.h"
#include "TransformInterface.h"
#include "AssetManager.h"

namespace Engine
{
	// Need to hide all dependencies from client
	class EMU
	{
	public:
		EMU_API static EMU* GetInstance();
		EMU_API inline void RunApp() { m_application.Start(); }
		EMU_API inline void EndApp() { m_application.End(); }

		// Scene Management
		EMU_API Scene& CreateScene(const std::string& name);
		EMU_API Scene& GetScene(const std::string& name) { return m_sceneManager.GetScene(name); }
		EMU_API void LoadScene(const std::string& name);
		EMU_API void UnloadCurrentScene();

		EMU_API PhysicsInterface& IPHYSICS() { return m_physicsInterface; }
		EMU_API CameraInterface& ICAMERA() { return m_cameraInterface; }
		EMU_API TransformInterface& ITRANSFORMS() { return m_transformInterface; }

		// ECS Interface functions
		EMU_API Entity CreateEntity();

		// Returns first entity with the given character.
		EMU_API Entity GetEntityByCharacter(const char c, const std::string& sceneName);

		// Returns vector of entities with the given character.
		EMU_API std::vector<Entity> GetEntitiesByCharacter(const char c, const std::string& sceneName);

		// Returns the current runtime entity with the given character.
		EMU_API Entity GetCurrentRuntimeEntity(const char c);

		// Activate all components in entity
		EMU_API void Activate(Entity entity);
		
		// Deactivate all components in entity
		EMU_API void Deactivate(Entity entity);

		// Cameras updated separately since there can only be one camera active.
		// EMU_API void ChangeCamera(Entity entity);

		// Activate/Deactivate specific components
		void ActivatePhysics(Entity entity);
		void DeactivatePhysics(Entity entity);

		void ActivateTransform(Entity entity);
		void DeactivateTransform(Entity entity);


		template<typename T, typename... Args>
		void AddComponent(Entity entity, Args&&... componentArgs)
		{
			m_ecs.AddComponent<T>(entity, std::forward<Args>(componentArgs)...);
		}

		template <typename T, typename... Args>
		void AddComponent(const char c, Args&&... componentArgs)
		{
			std::unordered_map<std::string, Scene>& scenes = m_sceneManager.GetAllScenes();
			for (auto& scene : scenes)
			{
				std::vector<Entity> entities = scene.second.GetTileMapEntities(c);
				for (Entity entity : entities)
				{
					m_ecs.AddComponent<T>(entity, std::forward<Args>(componentArgs)...);
				}
			}
		}

		// Event IO System Interface functions
		EMU_API void RegisterIOEventListener(IOEventType type, IOEventHandler handler);

	public:
		static void Init(const size_t numEntities);
		~EMU();
		
	private:
		EMU(const size_t numEntities);

		ECS m_ecs;
		AssetManager m_assetManager;

		PhysicsInterface m_physicsInterface;
		CameraInterface m_cameraInterface;
		TransformInterface m_transformInterface;

		IOEventSystem m_ioEventSystem;
		SceneManager m_sceneManager;
		Application m_application;
	};

} // namespace Engine