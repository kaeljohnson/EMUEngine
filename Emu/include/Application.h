#pragma once

#include "Core.h"
#include "Scenes/Scene.h"
#include "Camera/CameraManager.h"
#include "Events/EventManager.h"
#include "Rendering/WindowRenderer.h"

namespace Engine
{
	class Application
	{
	public:
		EMU_API static Application* GetInstance();
		EMU_API void PlayScene(std::shared_ptr<Scene> scene);
		EMU_API inline CameraManager& GetCameraManager() { return m_cameraManager; }
		EMU_API inline EventManager& GetEventManager() { return m_eventManager; }

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

		void defineDefaultApplicationCallbacks();
		void end();
	};
}