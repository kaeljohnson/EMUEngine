#pragma once

#include "../Public/Logger.h"
#include "../include/Events/IOEventSystem.h"
#include "../include/ISDL/ISDL.h"
#include "../Public/EMUTime.h"
#include "../include/Application.h"

namespace Engine
{

	Application::Application(ECS& refECS)
		: m_refECS(refECS), 
		m_sceneManager(m_refECS), 
		m_assetManager(), 
		m_audioSystem(m_refECS, m_assetManager),
		m_IRenderer(refECS, m_assetManager), 
		m_IOEventSystem(), 
		m_animationSystem(refECS)
	{
		m_IOEventSystem.Initialize();
	}
	void Application::Start()
	{
		Time::SetAppRunning(true);

		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		const float timeStep = Time::GetTimeStep();       // How much time to simulate each frame (in seconds)

		double currentTime = SDL_GetTicks64() / 1000.0;   // Get the current time in seconds
		double accumulator = 0.0;                         // How much time has passed since the last update

		double newTime = 0.0;
		double frameTime = 0.0;                           // How much time has passed since the last frame. How fast the game is running.
		float interpolation = 0.0;                        // How far between the last and current frame we are.


		// Application loop.
		while (Time::IsAppRunning())
		{
			// auto start = std::chrono::high_resolution_clock::now();
			m_sceneManager.IsSceneChanging();

			m_IOEventSystem.HandleEvents();
			m_IOEventSystem.ProcessEvents();

			newTime = SDL_GetTicks64() / 1000.0;
			frameTime = newTime - currentTime;

			currentTime = newTime;

			accumulator += frameTime;

			while (accumulator >= timeStep)
			{
				m_sceneManager.GetCurrentScene()->UpdatePhysics();
				m_animationSystem.Update();

				accumulator -= timeStep;
			}

			Time::SetInterpolationFactor(((float)accumulator / timeStep));

			m_sceneManager.GetCurrentScene()->UpdateCamera(m_assetManager);
			// m_refAudioSystem.PlayQueuedSound();

			m_IRenderer.CheckForWindowResizeRequest();
			m_IRenderer.Render();

			if (!Time::IsAppRunning())
			{ 
				m_sceneManager.Cleanup();
			}
			// auto end = std::chrono::high_resolution_clock::now();
			// std::chrono::duration<double, std::milli> elapsed = end - start;
			// std::cout << "Frame time: " << elapsed.count() << " ms\n";
		}
	}

	void Application::End()
	{
		ENGINE_LOG_D("Application ending!");

		Time::SetAppRunning(false);
	}

	Application::~Application() {}
}