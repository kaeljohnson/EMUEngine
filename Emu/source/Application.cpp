#pragma once

#include <stdio.h>
#include <queue>
#include <string>

#include "../include/Logging/Logger.h"
#include "../include/Application.h"
#include "../include/Events/Event.h"
#include "../include/Events/EventListenerStack.h"
#include "../include/Events/EventListener.h"
#include "../include/Scenes/SceneObject.h"
#include "../include/Scenes/Scene.h"
#include "../include/Physics/Box.h"
#include "../include/Physics/PhysicsFactory.h"

namespace Engine
{
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
		m_rendererManager(m_windowManager.getWindow()),
		m_eventManager(m_eventQ),
		running(false),
		m_pixelsPerMeter(20),
		m_timeStep(1.0f / 60.0f)
	{
		defineDefaultApplicationCallbacks();
	}

	std::shared_ptr<Scene> Application::CreateScene(const std::string& sceneName)
	{
		ENGINE_INFO_D("Creating scene: {}", sceneName);
		m_sceneMap[sceneName] = std::make_shared<Scene>(sceneName, m_timeStep, m_pixelsPerMeter);
		return m_sceneMap[sceneName];
	}

	void Application::SetTimeStep(const float timeStep)
	{
		m_timeStep = timeStep;
	}
	void Application::SetPixelsPerMeter(const int pixelsPerMeter)
	{
		m_pixelsPerMeter = pixelsPerMeter;
		for (auto& scene : m_sceneMap)
		{
			scene.second->m_pixelsPerMeter = pixelsPerMeter;
		}
	}

	void Application::PlayScene(std::string sceneName)
	{
		ENGINE_INFO_D("Playing scene: {}", sceneName);
		PlayScene(m_sceneMap[sceneName]);
	
	}

	void Application::PlayScene(std::shared_ptr<Scene> currentScene)
	{
		if (currentScene == nullptr)
		{
			ENGINE_CRITICAL_D("No scene loaded! Application must have a scene to run!");
			End();
		}

		currentScene->checkValid();

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

			while (accumulator >= m_timeStep)
			{
				m_eventManager.handleEvents();
				processEventQueue(currentScene);

				currentScene->update();

				accumulator -= m_timeStep;
			}

			const double interpolation = accumulator / m_timeStep;

			m_rendererManager.clearScreen();
			renderScene(currentScene, interpolation);
			m_rendererManager.display();      
		}
	}

	void Application::renderScene(std::shared_ptr<Scene> scene, const double interpolation)
	{
		// Should not be renderering every object in the scene every frame.

		size_t idx = 0;
		while (idx < scene->m_sceneObjectCount)
		{
			m_rendererManager.render(scene->m_sceneObjects[idx], m_pixelsPerMeter, interpolation);
			idx++;
		}

	}

	void Application::processEventQueue(std::shared_ptr<Scene> currentScene)
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

			for (auto& eventHandler : currentScene->m_eventListeners)
			{
				eventHandler->ProcessEvent(currentEvent);
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

	void Application::End()
	{
		ENGINE_INFO_D("Application ending!");

		running = false;
	}

	Application::~Application()
	{
		delete instance;
		instance = nullptr;
	}

	void Application::defineDefaultApplicationCallbacks()
	{
		ICallbackSystem* ptrICallbackSystem = ICallbackSystem::GetInstance();

		ptrICallbackSystem->NewCallback(Type::ToggleFullscreen, [this](Data data)
			{
				m_windowManager.toggleFullscreen();
				m_rendererManager.setViewport(m_windowManager.getWindow());
			});

		ptrICallbackSystem->NewCallback(Type::EndApplication, [this](Data data)
			{
				End();
			});

		ptrICallbackSystem->NewCallback(Type::ResizeWindow, [this](Data data)
			{
				const std::pair<int, int> windowSize = std::get<const std::pair<int, int>>(data);
				
				m_windowManager.resize(windowSize.first, windowSize.second);
				m_rendererManager.setViewport(m_windowManager.getWindow());
			});
	}
}