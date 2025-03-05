#pragma once

#include "../include/Logging/Logger.h"
#include "../include/Events/IOEvent.h"
#include "../include/Events/IOEventSystem.h"
#include "../include/Events/IOEventDispatcher.h"
#include "../include/Scenes/Scene.h"
#include "../include/Rendering/WindowRenderer.h"
#include "../include/ECS/ECS.h"
#include "../include/Components.h"
#include "../include/Includes.h"
#include "../include/Time.h"
#include "../include/Physics/Physics.h"
#include "../include/Physics/ContactSystem.h"
#include "../include/Application.h"

namespace Engine
{
	Application::Application()
		: m_cameraManager(), m_windowRenderer()
	{}

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
				ContactSystem::Cleanup();
				m_sceneManager.Cleanup();
				ECS::Cleanup();
			}
		}
	}

	void Application::End()
	{
		ENGINE_INFO_D("Application ending!");

		Time::SetAppRunning(false);
	}

	Application::~Application() {}
}