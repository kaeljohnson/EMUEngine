#pragma once

#include "Core.h"
#include "Application.h"
#include "Rendering/WindowRenderer.h"
#include "Scenes/SceneManager.h"
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
		EMU_API Entity* CreateEntity();
		EMU_API void Activate(Entity* ptrEntity);
		EMU_API void Deactivate(Entity* ptrEntity);
		template <typename T, typename... Args>
		void AddComponent(Entity* ptrEntity, Args&&... componentArgs)
		{
			m_ecs.AddComponent<T>(ptrEntity, std::forward<Args>(componentArgs)...);
		}

	public:
		static void Init();
		~EMU();
		
	private:
		EMU();

		ECS m_ecs;
		PhysicsInterface m_physicsInterface;
		CameraInterface m_cameraInterface;
		TransformInterface m_transformInterface;
		UpdatableInterface m_updatableInterface; // This may not be necessary.
		SceneManager m_sceneManager;
		Application m_application;
	};

} // namespace Engine