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
#include "../include/Scenes/SceneObject.h"
#include "../include/Scenes/Scene.h"
#include "../include/CallbackSystem/CallbackSystem.h"
#include "../include/RendererManager.h"

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
		: running(false), m_cameraManager(), m_eventManager()
	{
		RendererManager::GetInstance()->CreateRenderer();

		defineDefaultApplicationCallbacks();
	}

	void Application::PlayScene(std::shared_ptr<Scene> currentScene)
	{
		// Once sceme manager exists, this function will be a generic run funcion that queries the scene manager for the current scene.
		// Will need to add more functionality in here to handle scene switching.
		
		// renderer manager and event manager are singletons in order to hide dependencies from client.
		RendererManager* ptrRendererManager = RendererManager::GetInstance();

		// Camera frames current scene.
		m_cameraManager.m_ptrCurrentCamera->Frame(currentScene->GetPixelsPerMeter(), Vector2D<int>(currentScene->GetLevelWidthInMeters(), currentScene->GetLevelHeightInMeters()), 
			Vector2D<int>(ptrRendererManager->GetFullscreenWidth(), ptrRendererManager->GetFullscreenHeight()), ptrRendererManager->GetScale());


		ptrRendererManager->SetScene(currentScene);
		

		running = true;

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		if (currentScene == nullptr)
		{
			ENGINE_CRITICAL_D("No scene loaded! Application must have a scene to run!");
			end();
		}

		currentScene->CheckValid();

		const int pixelsPerMeter = currentScene->GetPixelsPerMeter();
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

			newTime = SDL_GetTicks() / 1000.0;
			frameTime = newTime - currentTime;
			currentTime = newTime;

			accumulator += frameTime;

			while (accumulator >= timeStep)
			{
				m_eventManager.HandleEvents();
				m_eventManager.ProcessEvents();
				
				currentScene->Update();
				
				accumulator -= timeStep;
			}

			interpolation = accumulator / timeStep;

			m_cameraManager.m_ptrCurrentCamera->Update(interpolation);

			ptrRendererManager->RenderScene(interpolation, m_cameraManager.m_ptrCurrentCamera->m_offset);
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