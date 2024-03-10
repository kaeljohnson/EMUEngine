#pragma once

#include <queue>

#include "WindowManager.h"
#include "RendererManager.h"
#include "Events/EventManager.h" 
#include "Events/Event.h"
#include "Events/IEventSystem.h"
#include "Layers/LayerStack.h"
#include "Layers/Layer.h"
#include "Actions/ActionTypes.h"

#include <variant>
#include <functional>

namespace Engine
{
	class Application
	{
	private:
		using EventData = std::variant<std::monostate, int, float, std::string, std::pair<int, int>>;

		// bool to indicate if the application is running or not.
		bool running;

		// Event queue to hold all events.
		std::queue<Event> m_eventQ;

		// All of the different managers that the application will use.
		WindowManager m_windowManager;
		RendererManager m_rendererManager;
		EventManager m_eventManager;

		// Event system for loose coupling between client code and application.
		IEventSystem m_eventSystem;

		// layer stack holds user defined layers.
		LayerStack m_layerStack;

		void processEventQueue();

	public:
		// Application constructor.
		Application(const char* appName);
		~Application() = default;

		// Event queue functions.
		void addToEventQ(Event& e);

		IEventSystem* getEventSystem();

		// Layer stack functions.
		void addToLayerStack(Layer* layer);
		void popLayerFromStack(std::string layerName);
		void popLayerFromStack();

		// Application functions.
		void run();
		void end();

		// Deleted functions to ensure our game instance cannot be copied or moved.
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;
	};
}