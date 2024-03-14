#pragma once

#include <queue>

#include "WindowManager.h"
#include "RendererManager.h"
#include "Events/EventManager.h" 
#include "Events/Event.h"
#include "Events/IEventAction.h"
#include "Layers/LayerStack.h"
#include "Layers/Layer.h"
#include "Layers/ApplicationLayer.h"
#include "Layers/WindowManagerLayer.h"
#include "box2d/box2d.h"
#include "box2d/b2_world.h"

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
		RendererManager m_rendererManager;
		EventManager m_eventManager;

		// Event system for loose coupling between client code and application.
		IEventAction m_eventActionInterface;

		ApplicationLayer m_appLayer;
		WindowManagerLayer m_windowManagerLayer;

		// layer stack holds user defined layers.
		LayerStack m_layerStack;

		// Box2D world.
		// Need to move this to a wrapper class.
		b2Vec2 m_gravity;
		b2World m_world;

		void processEventQueue();

	public:
		// Application constructor.
		Application(const char* appName);
		~Application() = default;

		IEventAction* getEventActionInterface();

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