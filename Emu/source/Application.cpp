#pragma once

#include <stdio.h>
#include <queue>
#include <thread>
#include <chrono>

#include "../include/Logging/Logger.h"
#include "../include/Application.h"
#include "../include/Events/IOEvent.h"
#include "../include/Events/IOEventSystem.h"
#include "../include/Events/IOEventDispatcher.h"
#include "../include/Scenes/Scene.h"
#include "../include/Time.h"
#include "../include/CallbackSystem/CallbackSystem.h"
#include "../include/Rendering/WindowRenderer.h"
#include "../include/ECS/ECS.h"
#include "../include/Components.h"

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
		: m_cameraManager(), m_windowRenderer()
	{
		defineDefaultApplicationCallbacks();
	}

	void Application::Start()
	{
		// Once sceme manager exists, this function will be a generic run funcion that queries the scene manager for the current scene.
		// Will need to add more functionality in here to handle scene switching.

		Time::SetAppRunning(true);

		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		const float timeStep = Time::GetTimeStep();

		double currentTime = SDL_GetTicks() / 1000.0;
		double accumulator = 0.0;

		double newTime = 0.0;
		double frameTime = 0.0;
		float interpolation = 0.0;

		// Application loop.
		while (Time::IsAppRunning())
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
				ECS::GetComponentManager<Camera>().GetComponent(m_cameraManager.m_ptrCurrentCameraEntity)->Frame(
					Vector2D<int>(m_sceneManager.GetCurrentScene()->GetLevelWidth(),
					m_sceneManager.GetCurrentScene()->GetLevelHeight()));
				m_sceneManager.NewSceneStarted();
			}

			newTime = SDL_GetTicks() / 1000.0;
			frameTime = newTime - currentTime;
			currentTime = newTime;

			accumulator += frameTime;

			while (accumulator >= timeStep)
			{
				IOEventSystem::HandleEvents();
				IOEventSystem::ProcessEvents();
				
				m_sceneManager.GetCurrentScene()->Update();

				accumulator -= timeStep;
			}

			Time::SetInterpolationFactor((float)accumulator / timeStep);

			// Camera system. Change to "CameraSystem".
			ECS::GetComponentManager<Updatable>().GetComponent(m_cameraManager.m_ptrCurrentCameraEntity)->Update();

			m_windowRenderer.Render(m_cameraManager.m_ptrCurrentCameraEntity);

			if (!Time::IsAppRunning())
			{ 
				// Cleanup static objects
				ECS::Cleanup();
			}
		}
	}

	void Application::end()
	{
		ENGINE_INFO_D("Application ending!");

		Time::SetAppRunning(false);
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