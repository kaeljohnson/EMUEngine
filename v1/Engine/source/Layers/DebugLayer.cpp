#pragma once

#include <queue>

#include "../../include/Events/Event.h"
#include "../../include/Layers/DebugLayer.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	DebugLayer::DebugLayer(std::queue<Event>& eventQ) : Layer(eventQ)
	{
	}

	void DebugLayer::processEvents()
	{
	}

	void DebugLayer::update()
	{
	}

	void DebugLayer::render()
	{
	}

	void DebugLayer::display()
	{
	}
}