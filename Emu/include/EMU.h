#pragma once

#include "Core.h"
#include "Application.h"
#include "Camera/CameraManager.h"
#include "Rendering/WindowRenderer.h"
#include "Scenes/SceneManager.h"

namespace Engine
{
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
		// EMU_API void RemoveScene(const std::string& name);
		// EMU_API Scene* GetScene(const std::string& name);

		// Camera Management
		EMU_API void SetCurrentCamera(Entity* name);

		EMU_API PhysicsInterface& PHYSICS() { return m_physicsInterface; }

	private:
		EMU();

		PhysicsInterface m_physicsInterface;

		CameraManager m_cameraManager;
		WindowRenderer m_windowRenderer;
		SceneManager m_sceneManager;

		Application m_application;
};

} // namespace Engine