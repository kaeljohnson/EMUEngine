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
	Application::Application(ECS& refECS, SceneManager& refSceneManager)
		: m_refECS(refECS), m_cameraSystem(refECS), m_windowRenderer(refECS), m_sceneManager(refSceneManager)
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
				m_sceneManager.NewSceneStarted();
			}

			IOEventSystem::HandleEvents();
			IOEventSystem::ProcessEvents();

			newTime = SDL_GetTicks() / 1000.0;
			frameTime = newTime - currentTime;
			currentTime = newTime;

			accumulator += frameTime;

			while (accumulator >= timeStep)
			{
				m_sceneManager.GetCurrentScene()->UpdateScripts();
				m_sceneManager.GetCurrentScene()->UpdatePhysics();

				accumulator -= timeStep;

				// ENGINE_INFO_D("1");
			}

			// ENGINE_INFO_D("2");

			Time::SetInterpolationFactor((float)accumulator / timeStep);

			
			m_sceneManager.GetCurrentScene()->UpdateVisuals();
			m_cameraSystem.Update();

			m_windowRenderer.Render();

			if (!Time::IsAppRunning())
			{ 
				m_sceneManager.Cleanup();
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