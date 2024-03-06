#pragma once

#include <stdio.h>
#include <queue>

#include "../include/Logging/Logger.h"

#include "../include/Application.h"
#include "../include/Events/Event.h"
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
		m_backgroundLayer(m_eventQ),
		m_gameLayer(m_eventQ),
		m_debugLayer(m_eventQ),
		m_foregroundLayer(m_eventQ),
		m_uiLayer(m_eventQ),
		running(false)
	{}

	void Application::run()
	{
		ENGINE_INFO("Application running!");

		running = true;

		// Application loop.
		while (running)
		{
			/*
			Each game loop iteration should:
				1. Handle events.
				2. Process actions.
				3. Clear the screen.
				4. Render game objects.
				5. Display the rendered game objects.
			*/

			m_eventManager.handleEvents();
			processEventQueue();

			m_rendererManager.clearScreen();
			// m_rendererManager.render(/* Rect */);
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
		// Background Layer -> Filled with Background textures.
		// Game Layer -> Filled with Engine supported GameObjects type.
		// Foreground Layer -> Filled with Foreground textures.
		// Debug Layer -> Wrapper for Game Layer. Shows important info like hit boxes, etc.
		// UI Layer -> Filled with Engine supported UI type.

		m_uiLayer.processEvents();
		m_debugLayer.processEvents();
		m_foregroundLayer.processEvents();
		m_gameLayer.processEvents();
		m_backgroundLayer.processEvents();

		m_windowManager.processEvents(m_eventQ);

		/*
			After all the layers process their events, the event 
			queue will be processed by the application to ensure all
			events are handled.
		*/

		while (!m_eventQ.empty())
		{
			const Event& currentEvent = m_eventQ.front();

			switch (currentEvent.m_eventType) 
			{
				case (QUIT): end(); break;
				case (ESCAPE_KEY_DOWN): end(); break;
				default: ENGINE_TRACE("Unhandled Event: {}", static_cast<int>(currentEvent.m_eventType)); break;
			}
			m_eventQ.pop();
		}
	}

	void Application::end()
	{
		running = false;
		m_rendererManager.free();
		m_windowManager.free();
	}

	void Application::addToEventQ(Event& e)
	{
		m_eventQ.push(e);
	}

	void Application::addToLayerStack(GameObject& gameObject)
	{
		m_gameLayer.push(gameObject);
	}
}