#pragma once

#include "../include/Logging/Logger.h"
#include "../include/Events/IOEventSystem.h"
#include "../include/ISDL/ISDL.h"
#include "../include/Time.h"
#include "../include/Application.h"

namespace Engine
{
	Application::Application(ECS& refECS, SceneManager& refSceneManager, IOEventSystem& refIOEventSystem)
		: m_refECS(refECS), m_ptrCurrentScene(nullptr), m_windowRenderer(refECS), 
		m_sceneManager(refSceneManager), m_refIOEventSystem(refIOEventSystem)
	{}

	void Application::Activate(Entity entity)
	{
		m_windowRenderer.Activate(entity);
	}

	void Application::Deactivate(Entity entity)
	{
		m_windowRenderer.Deactivate(entity);
	}

	void Application::Start()
	{
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
			if (m_sceneManager.IsNewSceneStarting())
			{	
				m_sceneManager.LoadQueuedScene();
				m_sceneManager.NewSceneStarted();
				m_ptrCurrentScene = m_sceneManager.GetCurrentScene();
				m_windowRenderer.Initialize();
			}

			m_refIOEventSystem.HandleEvents();
			m_refIOEventSystem.ProcessEvents();

			newTime = SDL_GetTicks() / 1000.0;
			frameTime = newTime - currentTime;
			currentTime = newTime;

			accumulator += frameTime;

			while (accumulator >= timeStep)
			{
				m_ptrCurrentScene->UpdateScripts();
				m_ptrCurrentScene->UpdatePhysics();

				accumulator -= timeStep;

				// ENGINE_INFO_D("1");
			}

			// ENGINE_INFO_D("2");

			Time::SetInterpolationFactor((float)accumulator / timeStep);

			m_ptrCurrentScene->UpdateVisuals();
			m_ptrCurrentScene->UpdateCamera();

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