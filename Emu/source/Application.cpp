#pragma once

#include <stdio.h>
#include <queue>
#include <thread>
#include <chrono>

#include "../include/EngineConstants.h"
#include "../include/Logging/Logger.h"
#include "../include/Application.h"
#include "../include/Events/Event.h"
#include "../include/Events/EventListener.h"
#include "../include/Events/EventManager.h"
#include "../include/Scenes/SceneObject.h"
#include "../include/Scenes/Scene.h"
#include "../include/CallbackSystem/CallbackSystem.h"
#include "../include/RendererManager.h"

namespace Engine
{
	void processEventQueue(std::shared_ptr<Scene> scene, EventManager* ptrEventManager, EventListener* ptrAppManagerListener); // Helper function for processing the event queue.

	// Application singleton.
	Application* Application::instance = nullptr;

	Application* Application::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new Application();
			return instance;
		}

		// Don't want client to reference
		// multiple instances of the application.
		ENGINE_CRITICAL_D("Warning! Instance of application already exists. Multiple usage only recommended for testing.");
		return instance;
	}

	Application::Application()
		: running(false), m_ptrAppManagerListener(nullptr), m_cameraManager()
	{
		RendererManager::GetInstance()->CreateRenderer();

		defineDefaultApplicationCallbacks();
	}

	void Application::PlayScene(std::shared_ptr<Scene> currentScene)
	{
		// Once sceme manager exists, this function will be a generic run funcion that queries the scene manager for the current scene.
		// Will need to add more functionality in here to handle scene switching.
		
		// renderer manager and event manager are singletons in order to hide dependencies from client.
		RendererManager* ptrRendererManager = RendererManager::GetInstance();
		EventManager* ptrEventManager = EventManager::GetInstance();

		// Camera frames current scene.
		m_cameraManager.m_ptrCurrentCamera->Frame(currentScene->GetPixelsPerMeter(), Vector2D<int>(currentScene->GetLevelWidthInMeters(), currentScene->GetLevelHeightInMeters()), 
			Vector2D<int>(ptrRendererManager->GetFullscreenWidth(), ptrRendererManager->GetFullscreenHeight()), ptrRendererManager->GetScale());


		ptrRendererManager->SetScene(currentScene);
		

		running = true;

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		if (currentScene == nullptr)
		{
			ENGINE_CRITICAL_D("No scene loaded! Application must have a scene to run!");
			end();
		}

		if (m_ptrAppManagerListener == nullptr)
		{
			ENGINE_CRITICAL_D("No event listener loaded! Application must have an event listener to run!");
			end();
		}

		currentScene->CheckValid();

		const int pixelsPerMeter = currentScene->GetPixelsPerMeter();
		const float timeStep = TIME_STEP;

		double currentTime = SDL_GetTicks() / 1000.0;
		double accumulator = 0.0;

		// Application loop.
		while (running)
		{
			/*
			Each game loop iteration should:
				1. Handle events.
				2. Process actions.
				3. Clear the screen.
				4. Render scene.
				5. Display the rendered scene.
			*/

			double newTime = SDL_GetTicks() / 1000.0;
			double frameTime = newTime - currentTime;
			currentTime = newTime;

			accumulator += frameTime;

			while (accumulator >= timeStep)
			{
				ptrEventManager->HandleEvents();
				processEventQueue(currentScene, ptrEventManager, m_ptrAppManagerListener);
				

				currentScene->Update();
				
				
				accumulator -= timeStep;
			}

			const double interpolation = accumulator / timeStep;

			m_cameraManager.m_ptrCurrentCamera->Update(interpolation);

			ptrRendererManager->RenderScene(interpolation, m_cameraManager.m_ptrCurrentCamera->m_offset);
		}
	}

	void Application::CreateEventListener(EventListener& eventListener)
	{
		ENGINE_INFO_D("Adding event listener to app.");

		m_ptrAppManagerListener = &eventListener;
	}

	void Application::end()
	{
		ENGINE_INFO_D("Application ending!");

		running = false;
	}

	Application::~Application() {}

	void Application::defineDefaultApplicationCallbacks()
	{
		ICallbackSystem* ptrICallbackSystem = ICallbackSystem::GetInstance();

		ptrICallbackSystem->NewCallback(Type::EndApplication, [&](Data data)
			{
				end();
			});
	}

	void processEventQueue(std::shared_ptr<Scene> scene, EventManager* ptrEventManager, EventListener* ptrAppManagerListener)
	{
		// Process order for scene is opposite of render order.

		// Potential for multithreading if there are a lot of events.

		// Render order for layers
		// EX:
		// Background Layer -> Filled with Background textures.
		// Game Layer -> Filled with Engine supported SceneObjects type.
		// Foreground Layer -> Filled with Foreground textures.
		// Debug Layer -> Wrapper for Game Layer. Shows important info like hit boxes, etc.
		// UI Layer -> Filled with Engine supported UI type.

		while (!ptrEventManager->eventQ.empty())
		{
			Event& currentEvent = ptrEventManager->eventQ.front();

			// Application events processed first.
			ptrAppManagerListener->ProcessEvent(currentEvent);

			for (auto& eventListener : scene->GetEventListeners())
			{
				eventListener->Enabled ? eventListener->ProcessEvent(currentEvent) : ENGINE_INFO_D("Event listener disabled");
				if (currentEvent.Handled)
				{
					break;
				}
			}

			/*if (!currentEvent.Handled)
			{
				ENGINE_TRACE_D("Unhandled Event: " + std::to_string(static_cast<int>(currentEvent.Type)));
			}*/

			ptrEventManager->eventQ.pop();
		}
	}
}