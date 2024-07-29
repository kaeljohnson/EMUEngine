#pragma once

#include <queue>
#include <unordered_map>
#include <memory>

#include "Core.h"
#include "Scenes/Scene.h"
#include "Camera/CameraManager.h"
#include "Events/EventManager.h"

namespace Engine
{
	class Application
	{
	public:
		EMU_API static Application* GetInstance();
		EMU_API void PlayScene(std::shared_ptr<Scene> scene);
		EMU_API inline CameraManager& GetCameraManager() { return m_cameraManager; }

		EventManager m_eventManager;

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

		// Application needs one listener for app management events.
		

		// This will exist here for now.
		// EventManager m_eventManager;
		CameraManager m_cameraManager;

		void defineDefaultApplicationCallbacks();
		void end();

		void processEventQueue();
	};
}