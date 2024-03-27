#pragma once

#include <queue>

#include "WindowManager.h"
#include "RendererManager.h"
#include "Events/EventManager.h" 
#include "Events/Event.h"
#include "Events/IEventAction.h"
#include "Layers/LayerStack.h"
#include "Layers/Layer.h"
#include "../include/Layers/ILayerEvent.h"
#include "Layers/ApplicationLayer.h"
#include "Layers/WindowManagerLayer.h"
#include "Physics/Box.h"
#include "Physics/World.h"

namespace Engine
{
	class Application
	{
	private:

		// bool to indicate if the application is running or not.
		bool running;

		// Event queue to hold all events.
		std::queue<Event> m_eventQ;

		// All of the different managers that the application will use.
		WindowManager m_windowManager;
		
		EventManager m_eventManager;

		// Event system for loose coupling between client code and application.
		IEventAction m_eventActionInterface;

		ApplicationLayer m_appLayer;
		WindowManagerLayer m_windowManagerLayer;
		RendererManager m_rendererManager;

		// layer stack holds user defined layers.
		LayerStack m_layerStack;

		ILayerEvent m_layerEventSystem;

		// Game loop management.
		const float timeStep;

		// Physics
		World m_world;

		void processEventQueue();
		void renderLayers();
		void defineDefaultApplicationCallbacks();

	public:
		// Application constructor.
		Application(const char* appName);
		~Application() = default;

		// Application should have just one callback interface. This interface
		// can utilize different types but there is no reason to separate the interfaces themselves
		// as they are just messaging systems.
		IEventAction* getEventActionInterface();
		ILayerEvent* getLayerEventInterface();

		// TEMP
		SDL_Renderer* getRenderer() { return m_rendererManager.getRenderer(); }

		// Layer stack functions.
		void pushToLayerStack(Layer* layer);
		void popLayerFromStack(Layer* layer);
		void popLayerFromStack();

		// Temp
		const LayerStack& getLayerStack() { return m_layerStack; }

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