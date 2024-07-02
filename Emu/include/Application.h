#pragma once

#include <queue>
#include <memory>

#include "Core.h"
#include "Events/EventListener.h"
#include "Physics/IWorld.h"
#include "Scenes/Scene.h"
#include "Camera/CameraManager.h"

namespace Engine
{
	class Application
	{
	public:
		EMU_API static Application* GetInstance();
		EMU_API void CreateEventListener(EventListener& eventListener);
		EMU_API void PlayScene(std::shared_ptr<Scene> scene);
		EMU_API inline CameraManager& GetCameraManager() { return m_cameraManager; }

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
		EventListener* m_ptrAppManagerListener;

		// This will exist here for now.
		CameraManager m_cameraManager;

		void defineDefaultApplicationCallbacks();
		void end();
	};
}