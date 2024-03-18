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
#include "../include/Physics/Box.h"

namespace Engine
{
	Application::Application(const char* appName)
		: m_windowManager(appName),
		m_rendererManager(m_windowManager.getWindow()),
		m_eventManager(m_eventQ),
		m_appLayer(&m_eventActionInterface),
		m_windowManagerLayer(&m_eventActionInterface),
		m_layerStack({&m_appLayer, &m_windowManagerLayer}),
		running(false),
		// The client will define the world.
		m_world(0.0f, 10.0f, 1.0f / 60.0f, 6, 2),
		tempGround({ 1, 50.0f, 1000.0f, 10.0f, 10.0f, 0.0f, 0.0f }),
		tempBox({ 2, 50.0f, 15.0f, 5.0f, 5.0f, 1.0f, 0.3f })
	{
		m_eventActionInterface.newActionCallback(ActionType::ToggleFullscreen, [this](EventData data)
		{
			m_windowManager.toggleFullscreen();
		});

		m_eventActionInterface.newActionCallback(ActionType::EndApplication, [this](EventData data)
		{
			end();
		});


		// Temp
		SDL_Surface* surface = SDL_CreateRGBSurface(0, 1000, 1000, 32, 0, 0, 0, 0);
		SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 0, 0));
		m_textureRed = SDL_CreateTextureFromSurface(m_rendererManager.getRenderer(), surface);
		SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 0));
		m_textureBlue = SDL_CreateTextureFromSurface(m_rendererManager.getRenderer(), surface);
		SDL_FreeSurface(surface);
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

		m_world.addBox(tempGround);
		m_world.addBox(tempBox);

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
			m_world.update();

			// Temp
			// The "+5" is due to the fact that box2ds collision detection runs a conservative resolution algorith, 
			// so the box2d body is slightly larger than the actual box. Therefore, visually, there is a gap between the box and the ground.
			// To fix this, we add 5 to the width and height of the box2d body, and render that size. This will need to be abstracted to separate
			// handling.
			SDL_Rect dynBody = { tempBox.getTopLeftX(), tempBox.getTopLeftY(), tempBox.getWidth() + 5, tempBox.getHeight() + 5 };
			SDL_Rect groundBody = { tempGround.getTopLeftX(), tempGround.getTopLeftY(), tempGround.getWidth() + 5, tempGround.getHeight() + 5 };

			m_rendererManager.clearScreen();
			m_rendererManager.render(groundBody, m_textureBlue);
			m_rendererManager.render(dynBody, m_textureRed);
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