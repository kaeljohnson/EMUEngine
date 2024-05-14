#pragma once

#include <queue>
#include <memory>

#include "Core.h"
#include "WindowManager.h"
#include "RendererManager.h"
#include "Events/EventManager.h" 
#include "Events/Event.h"
#include "Events/EventListenerStack.h"
#include "Events/EventListener.h"
#include "CallbackSystem/CallbackSystem.h"
#include "Physics/IWorld.h"
#include "Physics/PhysicsFactory.h"
#include "Scenes/Scene.h"

namespace Engine
{
	class Application
	{
	private:
		static Application* instance;
		Application();

		bool running;

		// Managers for major engine components.
		// Maybe decouple from application class?
		WindowManager m_windowManager;
		RendererManager m_rendererManager;
		
		// Event queue to hold all events.
		std::queue<Event> m_eventQ;
		// Manages events.
		// Maybe decouple from application class?
		EventManager m_eventManager;

		// Hold all event listeners.
		EventListenerStack m_eventListeners;

		void processEventQueue();
		void renderScene(std::shared_ptr<Scene> scene, const double interpolation);
		void defineDefaultApplicationCallbacks();

	public:
		EMU_API static Application* GetInstance();

		~Application();

		EMU_API void AddEventListener(EventListener* eventListener);

		// TEMP
	    SDLRenderer* GetRenderer() { return m_rendererManager.getRenderer(); }

		// Application functions.
		// EMU_API void PlayScene(std::string sceneName);
		EMU_API void PlayScene(std::shared_ptr<Scene> scene);
		EMU_API void End();

		// Deleted functions to ensure our app instance cannot be copied or moved.
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;
	};
}