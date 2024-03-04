#pragma once

#include <queue>

#include "../../include/Events/Event.h"
#include "../../include/Layers/BackgroundLayer.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	BackgroundLayer::BackgroundLayer(std::queue<Event>& eventQ) : Layer(eventQ)
	{
	}

	void BackgroundLayer::processEvents()
	{
	}

	void BackgroundLayer::update()
	{
	}

	void BackgroundLayer::render()
	{
	}

	void BackgroundLayer::display()
	{
	}
}