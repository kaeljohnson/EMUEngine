#pragma once

#include <string>

#include "../../include/Events/Event.h"
#include "../../include/Events/EventListener.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	EventListener::EventListener() : Enabled(true) {}

	void EventListener::ProcessEvent(Event& e) {};
}