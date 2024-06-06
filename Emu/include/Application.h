#pragma once

#include <queue>
#include <memory>

#include "Core.h"
#include "WindowManager.h"
#include "Rendering/RendererManager.h"
#include "Events/EventManager.h"
#include "Events/EventListener.h"
#include "Physics/IWorld.h"
#include "Scenes/Scene.h"

namespace Engine
{
	class Application
	{
	public:
		EMU_API static Application* GetInstance();
		EMU_API void CreateEventListener(EventListener& eventListener);
		EMU_API void PlayScene(std::shared_ptr<Scene> scene);

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

		// Managers for major engine components.
		// Maybe decouple from application class?
		WindowManager m_windowManager;

		RendererManager* ptrRendererManager;
		EventManager* ptrEventManager;

		// Application needs one listener for app management events.
		EventListener* m_ptrAppManagerListener;

		void processEventQueue(std::shared_ptr<Scene> scene);
		void renderScene(std::shared_ptr<Scene> scene, const double interpolation);
		void defineDefaultApplicationCallbacks();
		void end();

	
	};
}