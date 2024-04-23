#pragma once

#include <queue>
#include <memory>

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
#include "Scenes/Scene.h"

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

		// This will be replaced with a scene map. Where the client can load up the scene
		// they want to run. Scenes will hold the current level, and textures. They will
		// be big so they should not be hot at all times.
		std::shared_ptr<Scene> currentScene;

		void processEventQueue();
		// void renderScene();
		void renderLayers(const double interpolation);
		void defineDefaultApplicationCallbacks();

	public:
		EMU_API static Application* GetInstance();

		~Application();

		EMU_API std::shared_ptr<Scene> CreateScene(const std::string& sceneName);
		//EMU_API void SetSimulation(const float gravityX, const float gravityY, const float timeStep, const int pixelsPerMeter);
		EMU_API void SetTimeStep(const float timeStep);
		EMU_API void SetPixelsPerMeter(const int pixelsPerMeter);

		// TEMP
		EMU_API SDLRenderer* GetRenderer() { return m_rendererManager.getRenderer(); }

		// Push layer to layer stack. Pop layer from layer stack. This will change to being a scene member function.
		// The scene will own the layer stack.
		EMU_API void PushToLayerStack(Layer* layer);
		EMU_API void PopLayerFromStack(Layer* layer);
		EMU_API void PopLayerFromStack();

		// Application functions.
		// EMU_API PlayScene();
		EMU_API void Run();
		EMU_API void End();

		// Deleted functions to ensure our app instance cannot be copied or moved.
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;
	};
}