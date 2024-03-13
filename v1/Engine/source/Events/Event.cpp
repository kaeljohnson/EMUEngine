#pragma once

#include "../../include/Events/Event.h"

namespace Engine
{
	Event::Event(const EventType eventType) : Type(eventType), X_POS(-1), Y_POS(-1), Handled(false) {}
	Event::Event(const EventType eventType, const int xPos, const int yPos)
		: Type(eventType), X_POS(xPos), Y_POS(yPos), Handled(false)
	{}
}