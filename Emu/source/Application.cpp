#pragma once

#include <stdio.h>
#include <queue>

#include "../include/EngineConstants.h"
#include "../include/Logging/Logger.h"
#include "../include/Application.h"
#include "../include/Events/Event.h"
#include "../include/Events/EventListener.h"
#include "../include/Scenes/SceneObject.h"
#include "../include/Scenes/Scene.h"

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
		m_eventManager(m_eventQ),
		m_eventListeners(),
		running(false)
	{
		ptrRendererManager->CreateRenderer(m_windowManager.getWindow());

		defineDefaultApplicationCallbacks();
	}

	void Application::PlayScene(std::shared_ptr<Scene> currentScene)
	{
		if (currentScene == nullptr)
		{
			ENGINE_CRITICAL_D("No scene loaded! Application must have a scene to run!");
			End();
		}

		currentScene->checkValid();

		const int pixelsPerMeter = currentScene->GetPixelsPerMeter();
		const float timeStep = TIME_STEP;

		running = true;

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
				m_eventManager.handleEvents();
				processEventQueue();

				currentScene->update();

				accumulator -= timeStep;
			}

			const double interpolation = accumulator / timeStep;

			ptrRendererManager->clearScreen();
			renderScene(currentScene, interpolation);
			ptrRendererManager->display();
		}
	}

	void Application::renderScene(std::shared_ptr<Scene> scene, const double interpolation)
	{
		// Should not be renderering every object in the scene every frame.

		for (auto& sceneObject : *scene)
		{
			ptrRendererManager->render(sceneObject, scene->GetPixelsPerMeter(), interpolation);
		}

	}

	void Application::processEventQueue()
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

		while (!m_eventQ.empty())
		{
			Event& currentEvent = m_eventQ.front();

			for (auto& eventListener : m_eventListeners)
			{
				eventListener->ProcessEvent(currentEvent);
				if (currentEvent.Handled)
				{
					break;
				}
			}

			if (!currentEvent.Handled)
			{
				ENGINE_TRACE_D("Unhandled Event: {}", static_cast<int>(currentEvent.Type));
			}

			m_eventQ.pop();
		}
	}

	void Application::AddEventListener(EventListener* eventListener)
	{
		if (eventListener == nullptr)
		{
			ENGINE_WARN_D("EventListener is nullptr.");
			return;
		}

		ENGINE_INFO_D("Adding event listener to app.");

		m_eventListeners.push(eventListener);
	}

	void Application::End()
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
				m_windowManager.toggleFullscreen();
				ptrRendererManager->setViewport(m_windowManager.getWindow());
			});

		ptrICallbackSystem->NewCallback(Type::EndApplication, [this](Data data)
			{
				End();
			});

		ptrICallbackSystem->NewCallback(Type::ResizeWindow, [this](Data data)
			{
				const std::pair<int, int> windowSize = std::get<const std::pair<int, int>>(data);
				
				m_windowManager.resize(windowSize.first, windowSize.second);
				ptrRendererManager->setViewport(m_windowManager.getWindow());
			});
	}
}