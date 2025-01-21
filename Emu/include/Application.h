#pragma once

#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"
#include "Camera/CameraManager.h"
#include "Rendering/WindowRenderer.h"
#include "Core.h"

namespace Engine
{
	class Application
	{
	public:
		EMU_API Application(ECS& refECS, CameraManager& refCameraManager, WindowRenderer& refWindowRenderer, SceneManager& refSceneManager);
		EMU_API void Start();
		EMU_API inline CameraManager& GetCameraManager() { return m_cameraManager; }
		EMU_API inline SceneManager& GetSceneManager() { return m_sceneManager; }
		EMU_API void End();

		EMU_API ~Application();

		// Deleted functions to ensure our app instance cannot be copied or moved.
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;

	private:
		ECS& m_refECS;
		CameraManager& m_cameraManager;
		WindowRenderer& m_windowRenderer;
		SceneManager& m_sceneManager;
	};
}