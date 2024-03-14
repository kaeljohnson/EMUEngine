#pragma once

#include <stdio.h>
#include <queue>
#include <string>

#include "../include/Logging/Logger.h"

#include "../include/Application.h"
#include "../include/Events/Event.h"
#include "../include/Layers/Layer.h"
#include "../include/GameObjects/GameObject.h"
#include "../include/Layers/ApplicationLayer.h"
#include "../include/Layers/WindowManagerLayer.h"

#include "box2d/box2d.h"
#include "box2d/b2_world.h"

namespace Engine
{
	Application::Application(const char* appName)
		: m_windowManager(appName),
		m_rendererManager(m_windowManager.getWindow()),
		m_eventManager(m_eventQ),
		m_appLayer(&m_eventActionInterface),
		m_windowManagerLayer(&m_eventActionInterface),
		m_layerStack({&m_appLayer, &m_windowManagerLayer}),
		// Client should define gravity vector.
		m_gravity({ 0.0f, -10.f }),
		m_world(m_gravity),
		running(false)
	{
		m_eventActionInterface.newActionCallback(ActionType::ToggleFullscreen, [this](EventData data)
		{
			m_windowManager.toggleFullscreen();
		});

		m_eventActionInterface.newActionCallback(ActionType::EndApplication, [this](EventData data)
		{
			end();
		});
	}

	IEventAction* Application::getEventActionInterface() { return &m_eventActionInterface; }

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

			m_eventManager.handleEvents();
			processEventQueue();

			// update simulation.

			m_rendererManager.clearScreen();
			// m_rendererManager.render(m_layerStack);
			m_rendererManager.display();
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
	}

	void Application::popLayerFromStack(Layer* layer)
	{
		m_layerStack.popLayer(layer);
	}

	void Application::popLayerFromStack()
	{
		m_layerStack.popLayer();
	}
}