#pragma once

#include <queue>

#include "../../include/Events/Event.h"
#include "../../include/Layers/ForegroundLayer.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	ForegroundLayer::ForegroundLayer(std::queue<Event>& eventQ) : Layer(eventQ)
	{
	}

	void ForegroundLayer::processEvents()
	{
	}

	void ForegroundLayer::update()
	{
	}

	void ForegroundLayer::render()
	{
	}

	void ForegroundLayer::display()
	{
	}
}