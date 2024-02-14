#pragma once

#include "../../include/Events/Event.h"

namespace Engine
{
	Event::Event(const EventType eventType) : m_eventType(eventType), m_xPos(-1), m_yPos(-1) {}
	Event::Event(const EventType eventType, const int xPos, const int yPos)
		: m_eventType(eventType), m_xPos(xPos), m_yPos(yPos)
	{}
}