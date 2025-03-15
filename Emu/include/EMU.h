#pragma once

#include "Core.h"
#include "Application.h"
#include "Rendering/WindowRenderer.h"
#include "Scenes/SceneManager.h"
#include "Events/IOEventSystem.h"
#include "Physics/Physics.h"
#include "Camera/CameraInterface.h"
#include "TransformInterface.h"
#include "UpdatableInterface.h"

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
		EMU_API std::shared_ptr<Scene> CreateScene(const std::string& name);
		EMU_API void LoadScene(const std::string& name);
		EMU_API void UnloadCurrentScene();

		EMU_API PhysicsInterface& IPHYSICS() { return m_physicsInterface; }
		EMU_API CameraInterface& ICAMERA() { return m_cameraInterface; }
		EMU_API TransformInterface& ITRANSFORMS() { return m_transformInterface; }

		// ECS Interface functions
		EMU_API Entity CreateEntity();
		EMU_API void Activate(Entity entity);
		EMU_API void Deactivate(Entity entity);
		template <typename T, typename... Args>
		void AddComponent(Entity entity, Args&&... componentArgs)
		{
			m_ecs.AddComponent<T>(entity, std::forward<Args>(componentArgs)...);
		}

		// Event IO System Interface functions
		EMU_API void RegisterIOEventListener(IOEventType type, IOEventHandler handler);

	public:
		static void Init(const size_t numEntities);
		~EMU();
		
	private:
		EMU(const size_t numEntities);

		ECS m_ecs;
		PhysicsInterface m_physicsInterface;
		CameraInterface m_cameraInterface;
		TransformInterface m_transformInterface;
		UpdatableInterface m_updatableInterface; // This may not be necessary.

		IOEventSystem m_ioEventSystem;
		SceneManager m_sceneManager;
		Application m_application;
	};

} // namespace Engine