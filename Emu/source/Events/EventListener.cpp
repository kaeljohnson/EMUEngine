#pragma once

#include <string>

#include "../../include/Events/Event.h"
#include "../../include/Events/EventListener.h"

namespace Engine
{
	EventListener::EventListener() : Enabled(true) {}

	void EventListener::ProcessEvent(Event& e) {};
}