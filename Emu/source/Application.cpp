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
#include "../include/Scenes/SceneObject.h"
#include "../include/Scenes/Scene.h"
#include "../include/CallbackSystem/CallbackSystem.h"

namespace Engine
{
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
		: m_windowManager("Default App Name"),
		ptrRendererManager(RendererManager::GetInstance()),
		ptrEventManager(EventManager::GetInstance()),
		running(false), m_ptrAppManagerListener(nullptr)
	{
		ptrRendererManager->CreateRenderer(m_windowManager.GetWindow());

		defineDefaultApplicationCallbacks();
	}

	void Application::PlayScene(std::shared_ptr<Scene> currentScene)
	{
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
				processEventQueue(currentScene);

				currentScene->Update();

				accumulator -= timeStep;
			}

			const double interpolation = accumulator / timeStep;

			ptrRendererManager->ClearScreen();
			renderScene(currentScene, interpolation);
			ptrRendererManager->Display();
		}
	}

	void Application::renderScene(std::shared_ptr<Scene> scene, const double interpolation)
	{
		// Should not be rendering every object in the scene every frame.

		for (auto& sceneObject : *scene)
		{
			ptrRendererManager->Render(sceneObject, scene->GetPixelsPerMeter(), interpolation);
		}
	}

	void Application::processEventQueue(std::shared_ptr<Scene> scene)
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
			m_ptrAppManagerListener->ProcessEvent(currentEvent);

			for (auto& eventListener : scene->GetEventListeners())
			{
				eventListener->Enabled ? eventListener->ProcessEvent(currentEvent) : ENGINE_INFO_D("Event listener disabled");
				if (currentEvent.Handled)
				{
					break;
				}
			}

			if (!currentEvent.Handled)
			{
				ENGINE_TRACE_D("Unhandled Event: {}", static_cast<int>(currentEvent.Type));
			}

			ptrEventManager->eventQ.pop();
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

		ptrICallbackSystem->NewCallback(Type::ToggleFullscreen, [this](Data data)
			{
				m_windowManager.ToggleFullscreen();
				ptrRendererManager->SetViewport(m_windowManager.GetWindow());
			});

		ptrICallbackSystem->NewCallback(Type::EndApplication, [this](Data data)
			{
				end();
			});

		ptrICallbackSystem->NewCallback(Type::ResizeWindow, [this](Data data)
			{
				const std::pair<int, int> windowSize = std::get<const std::pair<int, int>>(data);
				
				m_windowManager.Resize(windowSize.first, windowSize.second);
				ptrRendererManager->SetViewport(m_windowManager.GetWindow());
			});
	}
}