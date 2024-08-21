#pragma once

#include "Core.h"
#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"
#include "Camera/CameraManager.h"
#include "Events/EventManager.h"
#include "Rendering/WindowRenderer.h"

namespace Engine
{
	class Application
	{
	public:
		EMU_API static Application* GetInstance();
		EMU_API void Start();
		EMU_API inline CameraManager& GetCameraManager() { return m_cameraManager; }
		EMU_API inline EventManager& GetEventManager() { return m_eventManager; }
		EMU_API inline SceneManager& GetSceneManager() { return m_sceneManager; }

		~Application();

		// Deleted functions to ensure our app instance cannot be copied or moved.
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;

	private:
		static Application* instance;
		Application();

		bool running;

		CameraManager m_cameraManager;
		EventManager m_eventManager;
		WindowRenderer m_windowRenderer;
		SceneManager m_sceneManager;

		void defineDefaultApplicationCallbacks();
		void end();
	};
}