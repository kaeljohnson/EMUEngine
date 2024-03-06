#pragma once

#include <queue>

#include "../../include/Events/Event.h"
#include "../../include/Layers/GameLayer.h"
#include "../../include/Logging/Logger.h"
#include "../../include/GameObjects/GameObject.h"

namespace Engine
{
	GameLayer::GameLayer(std::queue<Event>& eventQ) : Layer(eventQ)
	{
	}

	void GameLayer::push(GameObject& gameObject)
	{
		m_gameObjects.push_back(gameObject);
	}

	void GameLayer::processEvents()
	{
		for (auto& gameObject : m_gameObjects)
		{
			gameObject.processEvents(refEventQ);
		}
	}

	void GameLayer::update()
	{
	}

	void GameLayer::render()
	{
	}


	void GameLayer::display()
	{
	}
}