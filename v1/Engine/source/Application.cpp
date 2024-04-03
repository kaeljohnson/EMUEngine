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

namespace Engine
{
	Application::Application()
		: m_windowManager("Default App Name"),
		m_rendererManager(m_windowManager.getWindow()),
		m_eventManager(m_eventQ),
		m_layerStack(),
		running(false),
		m_timeStep(1.0f / 60.0f),
		m_world(0.0f, 9.8f, m_timeStep, 6, 2)
	{
		defineDefaultApplicationCallbacks();
	}

	void Application::createSimulation(float gravityX, float gravityY, float timeStep)
	{
		m_timeStep = timeStep;
		m_world.SetGravity(gravityX, gravityY);
		m_world.SetTimeStep(timeStep);
		m_world.SetVelocityIterations(6);
		m_world.SetPositionIterations(2);

		ENGINE_INFO("Client creating simulation with gravity: ({}, {}) and time step: {}", gravityX, gravityY, timeStep);
	}

	Application* Application::getInstance()
	{
		if (instance == nullptr)
		{
			instance = new Application();
			return instance;
		}
		
		// Don't want client to reference 
		// multiple instances of the application.
		ENGINE_CRITICAL("Warning! Instance of application already exists. Multiple usage only recommended for testing.");
		return instance;
	}

	void Application::run()
	{

		for (auto& layer : m_layerStack)
		{
			ENGINE_TRACE("Layer: {}", layer->getName());
		}

		ENGINE_INFO("Application running!");

		if (m_layerStack.size() > 1)
		{
			ENGINE_TRACE("Layer stack size: {}", m_layerStack.size());
		}
		else
		{
			ENGINE_CRITICAL("Layer stack is empty! Application must have at least two layers to be valid!");
			end();
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
			
				// update simulation.
				m_world.update();

				accumulator -= m_timeStep;
			}

			// Calculate interpolation factor. This will be used in the future.
			const double interpolation = accumulator / m_timeStep;

			m_rendererManager.clearScreen();

			// Need interpolation for smooth rendering.
			renderLayers();
			m_rendererManager.display();      
		}
	}

	void Application::renderLayers()
	{
		for (Layer* layer : m_layerStack)
		{
			for (GameObject* gameObject : *layer)
			{
				m_rendererManager.render(gameObject);
			}
		}
	}

	void Application::processEventQueue()
	{
		// Process order for layers is opporsite of render order.
		
		// Render order for layers:
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
				
				(*--it_layer)->processEvent(currentEvent);
				if (currentEvent.Handled)
				{
					break;
				}
			}

			if (!currentEvent.Handled)
			{
				ENGINE_TRACE("Unhandled Event: {}", static_cast<int>(currentEvent.Type));
			}

			m_eventQ.pop();
		}
	}

	void Application::end()
	{
		ENGINE_INFO("Application ending!");

		running = false;
	}

	void Application::pushToLayerStack(Layer* layer)
	{
		m_layerStack.pushLayer(layer);
		layer->isAttached = true;
	}

	void Application::popLayerFromStack(Layer* layer)
	{
		m_layerStack.popLayer(layer);
		layer->isAttached = false;
	}

	void Application::popLayerFromStack()
	{
		m_layerStack.popLayer();
	}

	void Application::defineDefaultApplicationCallbacks()
	{
		ICallbackSystem* ptrICallbackSystem = ICallbackSystem::getInstance();

		ptrICallbackSystem->newCallback(Type::ToggleFullscreen, [this](Data data)
			{
				m_windowManager.toggleFullscreen();
			});

		ptrICallbackSystem->newCallback(Type::EndApplication, [this](Data data)
			{
				end();
			});

		ptrICallbackSystem->newCallback(Type::AddToWorld, [this](Data layerEventData)
			{
				GameObject* gameObject = std::get<GameObject*>(layerEventData);

				m_world.addBox(*gameObject);
			});

		ptrICallbackSystem->newCallback(Type::RemoveFromWorld, [this](Data layerEventData)
			{
				GameObject* gameObject = std::get<GameObject*>(layerEventData);

				m_world.removeBox(*gameObject);
			});
	}
}