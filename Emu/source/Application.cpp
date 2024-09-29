#pragma once

#include <stdio.h>
#include <queue>
#include <thread>
#include <chrono>

#include "../include/EngineConstants.h"
#include "../include/Logging/Logger.h"
#include "../include/Application.h"
#include "../include/Events/Event.h"
#include "../include/Events/EventManager.h"
#include "../include/Events/EventDispatcher.h"
#include "../include/Scenes/Scene.h"
#include "../include/CallbackSystem/CallbackSystem.h"
#include "../include/Rendering/WindowRenderer.h"

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
		: running(false), m_cameraManager(), m_eventManager(), m_windowRenderer()
	{
		defineDefaultApplicationCallbacks();
	}

	void Application::Start()
	{
		// Once sceme manager exists, this function will be a generic run funcion that queries the scene manager for the current scene.
		// Will need to add more functionality in here to handle scene switching.

		m_windowRenderer.SetCamera(m_cameraManager.m_ptrCurrentCamera);

		running = true;

		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		const float timeStep = TIME_STEP;

		double currentTime = SDL_GetTicks() / 1000.0;
		double accumulator = 0.0;

		double newTime = 0.0;
		double frameTime = 0.0;
		double interpolation = 0.0;

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

			if (m_sceneManager.IsNewSceneStarting())
			{
				// Camera frames current scene.
				m_cameraManager.m_ptrCurrentCamera->Frame(Vector2D<int>(m_sceneManager.GetCurrentScene()->GetLevelWidth(), 
					m_sceneManager.GetCurrentScene()->GetLevelHeight()));
				m_sceneManager.NewSceneStarted();
			}

			newTime = SDL_GetTicks() / 1000.0;
			frameTime = newTime - currentTime;
			currentTime = newTime;

			accumulator += frameTime;

			while (accumulator >= timeStep)
			{
				m_eventManager.HandleEvents();
				m_eventManager.ProcessEvents();
				
				m_sceneManager.GetCurrentScene()->Update();
				
				accumulator -= timeStep;
			}

			interpolation = accumulator / timeStep;

			m_cameraManager.m_ptrCurrentCamera->Update(interpolation);

			m_windowRenderer.RenderScene(m_sceneManager.GetCurrentScene(), interpolation);
		}
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
}