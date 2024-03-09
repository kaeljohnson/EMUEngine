#pragma once

#include <stdio.h>
#include <queue>

#include "../include/Logging/Logger.h"

#include "../include/Application.h"
#include "../include/Events/Event.h"
#include "../include/Layers/Layer.h"
#include "../include/GameObjects/GameObject.h"

namespace Engine
{
	// Definition for Game constructor. Note: "<Class>::<Function>" the function
	// on the right side of the "::" comes from the class on the left side.
	Application::Application(const char* appName)
		/* 
			Initialization list for the Application constructor.
			Initialization lists are more optimal than setting 
			variables within the square brackets of the constructor.
		*/
		: m_windowManager(appName),
		m_rendererManager(m_windowManager.getWindow()),
		m_eventManager(m_eventQ),
		running(false)
	{
		m_eventManager.newEventCallback(ActionType::ToggleFullscreen, [this]() 
		{
			m_windowManager.toggleFullscreen();
		});

	}

	void Application::newEventCallback(const ActionType actionType, EventCallback callback)
	{
		m_eventManager.newEventCallback(actionType, callback);
	}

	void Application::triggerEventCallback(const ActionType actionType)
	{
		m_eventManager.triggerEventCallback(actionType);
	}

	void Application::run()
	{
		ENGINE_INFO("Application running!");

		if (m_layerStack.size() > 0)
		{
			ENGINE_TRACE("Layer stack size: {}", m_layerStack.size());
		}
		else
		{
			ENGINE_CRITICAL("Layer stack is empty! Application must have at least one layer to be valid!");
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

			m_rendererManager.clearScreen();
			// m_rendererManager.render(m_layerStack);
			m_rendererManager.display();
		}
	}

	void Application::processEventQueue()
	{
		/*
			Each layer will have its own processEvents function.
			These functions will be called in the order that the layers are added.
			The user needs to decide in what order layers are added.
			This is because the user needs to decide which layer has priority over the others,
			and which layer should handle the events first. The user also needs to determine
			what order the layers should be rendered in.
		*/

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
				if (currentEvent.handled)
				{
					break;
				}
			}

			if (!currentEvent.handled)
			{

				// For now, put any non-layer event handling here.
				// m_windowManager.processEvent(currentEvent);

				switch (currentEvent.m_eventType)
				{
					case (QUIT): end(); break;
					case (ESCAPE_KEY_DOWN): end(); break;
					default: ENGINE_TRACE("Unhandled Event: {}", static_cast<int>(currentEvent.m_eventType)); break;
				}
			}
			m_eventQ.pop();
		}
	}

	WindowManager* Application::getWindowManager()
	{
		return &m_windowManager;
	}

	void Application::end()
	{
		ENGINE_INFO("Application ending!");

		running = false;
	}

	void Application::addToEventQ(Event& e)
	{
		m_eventQ.push(e);
	}

	void Application::addToLayerStack(Layer* layer)
	{
		if (layer == nullptr) return;
		for (auto it = m_layerStack.begin(); it != m_layerStack.end(); ++it)
		{
			if ((*it)->getName() == layer->getName())
			{
				ENGINE_CRITICAL("Layer with name: {} already exists in the layer stack!", layer->getName());
				return;
			}
		}

		m_layerStack.pushLayer(layer);
	}

	void Application::popLayerFromStack(std::string layerName)
	{
		m_layerStack.popLayer(layerName);
	}

	void Application::popLayerFromStack()
	{
		m_layerStack.popLayer();
	}
}