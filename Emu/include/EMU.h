#pragma once

#include "Core.h"
#include "Application.h"
#include "Camera/CameraManager.h"
#include "Rendering/WindowRenderer.h"
#include "Scenes/SceneManager.h"

namespace Engine
{
	// Need to hide all dependencies from client
	class EMU
	{
	public:
		static void Init();
		EMU_API static EMU* GetInstance();
		~EMU();
		EMU_API inline void RunApp() { m_application.Start(); }
		EMU_API inline void EndApp() { m_application.End(); }

		// Scene Management
		EMU_API std::shared_ptr<Scene> CreateScene(const std::string& name);
		EMU_API void LoadScene(const std::string& name);
		EMU_API void UnloadCurrentScene();

		EMU_API PhysicsInterface& PHYSICS() { return m_physicsInterface; }
		EMU_API CameraInterface& ICAMERA() { return m_cameraInterface; }
		EMU_API ECS& IECS() { return m_ecs; }

		// ECS Interface functions
		EMU_API Entity* CreateEntity();
		template <typename T, typename... Args>
		void AddComponent(Entity* ptrEntity, Args&&... componentArgs)
		{
			m_ecs.AddComponent<T>(ptrEntity, std::forward<Args>(componentArgs)...);
		}

	private:
		EMU();

		ECS m_ecs;
		// Separate contact system into ContactsManager and ContactSystem
		PhysicsInterface m_physicsInterface;
		CameraInterface m_cameraInterface;
		SceneManager m_sceneManager;
		Application m_application;
	};

} // namespace Engine