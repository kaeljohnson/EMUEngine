#pragma once

#include <queue>

#include "Core.h"
#include "WindowManager.h"
#include "RendererManager.h"
#include "Events/EventManager.h" 
#include "Events/Event.h"
#include "Layers/LayerStack.h"
#include "Layers/Layer.h"
#include "CallbackSystem/CallbackSystem.h"
#include "Physics/IWorld.h"
#include "Physics/PhysicsFactory.h"

namespace Engine
{
	class Application
	{
	private:
		static Application* instance;
		Application();

		bool running;

		// Time step for physics simulation. Can be set by user.
		float m_timeStep;

		int m_pixelsPerMeter;

		// Event queue to hold all events.
		std::queue<Event> m_eventQ;

		// Managers for major engine components.
		WindowManager m_windowManager;
		RendererManager m_rendererManager;
		
		// Manages events.
		EventManager m_eventManager;

		// Hold applications layers.
		LayerStack m_layerStack;

		// Interface for world where the application simulation exists within.
		IWorld* m_world;

		void processEventQueue();
		void renderLayers(const double interpolation);
		void defineDefaultApplicationCallbacks();

	public:
		EMU_API static Application* GetInstance();

		~Application();

		EMU_API void SetSimulation(const float gravityX, const float gravityY, const float timeStep, const int pixelsPerMeter);

		// TEMP
		EMU_API SDLRenderer* GetRenderer() { return m_rendererManager.getRenderer(); }

		// Push layer to layer stack. Pop layer from layer stack. This will change to being a scene member function.
		// The scene will own the layer stack.
		EMU_API void PushToLayerStack(Layer* layer);
		EMU_API void PopLayerFromStack(Layer* layer);
		EMU_API void PopLayerFromStack();

		// Application functions.
		EMU_API void Run();
		EMU_API void End();

		// Deleted functions to ensure our app instance cannot be copied or moved.
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;
	};
}