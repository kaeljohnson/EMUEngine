#pragma once

#include <queue>

#include "WindowManager.h"
#include "RendererManager.h"
#include "Events/EventManager.h" 
#include "Events/Event.h"
#include "Layers/LayerStack.h"
#include "Layers/Layer.h"
#include "CallbackSystem/CallbackSystem.h"
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
		RendererManager m_rendererManager;
		
		EventManager m_eventManager;

		// layer stack holds user defined layers.
		LayerStack m_layerStack;

		// Game loop management.
		const float timeStep;

		// Physics
		World m_world;

		void processEventQueue();
		void renderLayers();
		void defineDefaultApplicationCallbacks();

	public:
		// Application constructor.
		Application();
		~Application() = default;

		ICallbackSystem CallbackSystem;

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