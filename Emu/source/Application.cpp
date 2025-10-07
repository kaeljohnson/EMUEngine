#pragma once

#include "../include/Logging/Logger.h"
#include "../include/Events/IOEventSystem.h"
#include "../include/ISDL/ISDL.h"
#include "../include/Time.h"
#include "../include/Application.h"

namespace Engine
{

	Application::Application(ECS& refECS, SceneManager& refSceneManager, IOEventSystem& refIOEventSystem, 
		AssetManager& refAssetManager, AudioSystem& refAudioSystem, AnimationSystem& refAnimationSystem)
		: m_refECS(refECS), m_refAssetManager(refAssetManager), m_refAudioSystem(refAudioSystem), m_ptrCurrentScene(nullptr), 
		m_IRenderer(refECS, refAssetManager), m_refSceneManager(refSceneManager), m_refIOEventSystem(refIOEventSystem), m_refAnimationSystem(refAnimationSystem)
	{}

	void Application::Activate(Entity entity)
	{
	}

	void Application::Deactivate(Entity entity)
	{
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
			if (m_refSceneManager.IsNewSceneStarting())
			{	
				ENGINE_INFO_D("New scene starting!");
				m_refSceneManager.LoadQueuedScene();
				m_refSceneManager.NewSceneStarted();
				m_ptrCurrentScene = m_refSceneManager.GetCurrentScene();

				// Critical checks before scene starts.
				if (m_refECS.GetHotComponents<Camera>().empty())
				{
					ENGINE_CRITICAL_D("No active cameras found.");
					throw std::runtime_error("No active cameras found.");
					return;
				}
			}

			m_refIOEventSystem.HandleEvents();
			m_refIOEventSystem.ProcessEvents();

			newTime = SDL_GetTicks64() / 1000.0;
			frameTime = newTime - currentTime;

			currentTime = newTime;

			accumulator += frameTime;

			while (accumulator >= timeStep)
			{
				m_ptrCurrentScene->UpdatePhysics();
				m_refAnimationSystem.Update(timeStep);

				accumulator -= timeStep;
			}

			Time::SetInterpolationFactor(((float)accumulator / timeStep));

			m_ptrCurrentScene->UpdateCamera(m_refAssetManager);
			// m_refAudioSystem.PlayQueuedSound();

			m_IRenderer.CheckForWindowResizeRequest();
			m_IRenderer.Render();

			if (!Time::IsAppRunning())
			{ 
				m_refSceneManager.Cleanup();
			}
			// auto end = std::chrono::high_resolution_clock::now();
			// std::chrono::duration<double, std::milli> elapsed = end - start;
			// std::cout << "Frame time: " << elapsed.count() << " ms\n";
		}
	}

	void Application::End()
	{
		ENGINE_INFO_D("Application ending!");

		Time::SetAppRunning(false);
	}

	Application::~Application() {}
}