#pragma once

#include <queue>

#include "../../include/Events/Event.h"
#include "../../include/Layers/UILayer.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	UILayer::UILayer(std::queue<Event>& eventQ) : Layer(eventQ)
	{
	}

	void UILayer::processEvents()
	{
	}

	void UILayer::update()
	{
	}

	void UILayer::render()
	{
	}

	void UILayer::display()
	{
	}
}