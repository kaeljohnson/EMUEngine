#pragma once

#include <stdio.h>
#include <queue>
#include <string>

#include "../include/Logging/Logger.h"
#include "../include/Application.h"
#include "../include/Events/Event.h"
#include "../include/Layers/Layer.h"
#include "../include/GameObjects/GameObject.h"
#include "../include/Physics/Box.h"
#include "../include/Physics/PhysicsFactory.h"

namespace Engine
{
	Application::Application()
		: m_windowManager("Default App Name"),
		m_rendererManager(m_windowManager.getWindow()),
		m_eventManager(m_eventQ),
		m_layerStack(),
		running(false),
		m_pixelsPerMeter(20),
		m_timeStep(1.0f / 60.0f)
	{

		m_world = CreateWorld(0.0f * m_pixelsPerMeter, 9.8f * m_pixelsPerMeter, m_timeStep, 8, 3);
		defineDefaultApplicationCallbacks();
	}

	void Application::SetSimulation(const float gravityX, const float gravityY, const float timeStep, const int pixelsPerMeter)
	{
		m_timeStep = timeStep;
		m_pixelsPerMeter = pixelsPerMeter;
		m_world->SetGravity(gravityX * pixelsPerMeter, gravityY * pixelsPerMeter);
		m_world->SetTimeStep(timeStep);
		m_world->SetVelocityIterations(6);
		m_world->SetPositionIterations(2);

		ENGINE_INFO_D("Client creating simulation with gravity: ({}, {}) and time step: {}", gravityX, gravityY, timeStep);
	}

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

	void Application::Run()
	{

		for (auto& layer : m_layerStack)
		{
			ENGINE_TRACE_D("Layer: {}", layer->GetName());
		}

		ENGINE_INFO("Application running!");

		if (m_layerStack.size() > 0)
		{
			ENGINE_TRACE_D("Layer stack size: {}", m_layerStack.size());
		}
		else
		{
			ENGINE_CRITICAL_D("Layer stack is empty! Application must have at least one layers to be valid!");
			End();
		}

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
				4. Render layer.
				5. Display the rendered layers.
			*/

			double newTime = SDL_GetTicks() / 1000.0;
			double frameTime = newTime - currentTime;
			currentTime = newTime;

			accumulator += frameTime;

			while (accumulator >= m_timeStep)
			{
				m_eventManager.handleEvents();
				processEventQueue();

				// Faster way to do this? Should only have to update objects
				// prev values if they have changed. In fact, should only update
				// objects that have changed in general
				for (Layer* layer : m_layerStack)
				{
					for (GameObject* gameObject : *layer)
					{
						gameObject->GetPhysicsBody()->updatePrevX();
						gameObject->GetPhysicsBody()->updatePrevY();
					}
				}

				m_world->update();

				accumulator -= m_timeStep;
			}

			// Calculate interpolation factor. This will be used in the future.
			const double interpolation = accumulator / m_timeStep;

			m_rendererManager.clearScreen();

			// Need interpolation for smooth rendering.
			renderLayers(interpolation);
			m_rendererManager.display();      
		}
	}

	void Application::renderLayers(const double interpolation)
	{
		// Should not be renderering every layer every frame.
	    // Should only render layers that are visible and have changed.

		for (Layer* layer : m_layerStack)
		{
			for (GameObject* gameObject : *layer)
			{
				m_rendererManager.render(gameObject, m_pixelsPerMeter, interpolation);
			}
		}
	}

	void Application::processEventQueue()
	{
		// Process order for layers is opporsite of render order.

		// Potential for multithreading if there are a lot of events.
		
		// Render order for layers
		// EX:
		// Background Layer -> Filled with Background textures.
		// Game Layer -> Filled with Engine supported GameObjects type.
		// Foreground Layer -> Filled with Foreground textures.
		// Debug Layer -> Wrapper for Game Layer. Shows important info like hit boxes, etc.
		// UI Layer -> Filled with Engine supported UI type.

		while (!m_eventQ.empty())
		{
			Event& currentEvent = m_eventQ.front();
			for (auto it_layer = m_layerStack.end(); it_layer != m_layerStack.begin();)
			{
				
				(*--it_layer)->ProcessEvent(currentEvent);
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
		ENGINE_INFO("Application ending!");

		running = false;
	}

	void Application::PushToLayerStack(Layer* layer)
	{
		m_layerStack.pushLayer(layer);
		layer->isAttached = true;
	}

	void Application::PopLayerFromStack(Layer* layer)
	{
		m_layerStack.popLayer(layer);
		layer->isAttached = false;
	}

	void Application::PopLayerFromStack()
	{
		m_layerStack.popLayer();
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

		ptrICallbackSystem->NewCallback(Type::AddToWorld, [this](Data layerEventData)
			{
				GameObject* gameObject = std::get<GameObject*>(layerEventData);
				Box* ptrBox = static_cast<Box*>(gameObject->GetPhysicsBody());

				m_world->addBox(ptrBox);
			});

		ptrICallbackSystem->NewCallback(Type::RemoveFromWorld, [this](Data layerEventData)
			{
				GameObject* gameObject = std::get<GameObject*>(layerEventData);
				Box* ptrBox = static_cast<Box*>(gameObject->GetPhysicsBody());

				m_world->removeBox(ptrBox);
			});

		ptrICallbackSystem->NewCallback(Type::ResizeWindow, [this](Data data)
			{
				const std::pair<int, int> windowSize = std::get<const std::pair<int, int>>(data);
				
				m_windowManager.resize(windowSize.first, windowSize.second);
				m_rendererManager.setViewport(m_windowManager.getWindow());
			});
	}
}