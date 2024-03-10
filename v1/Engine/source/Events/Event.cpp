#pragma once

#include "../../include/Events/Event.h"

namespace Engine
{
	Event::Event(const EventType eventType) : eventType(eventType), xPos(-1), yPos(-1), handled(false) {}
	Event::Event(const EventType eventType, const int xPos, const int yPos)
		: eventType(eventType), xPos(xPos), yPos(yPos), handled(false)
	{}
}