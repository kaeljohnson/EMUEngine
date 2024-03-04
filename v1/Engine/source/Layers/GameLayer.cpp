#pragma once

#include <queue>

#include "../../include/Events/Event.h"
#include "../../include/Layers/GameLayer.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	GameLayer::GameLayer(std::queue<Event>& eventQ) : Layer(eventQ)
	{
	}

	void GameLayer::processEvents()
	{
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