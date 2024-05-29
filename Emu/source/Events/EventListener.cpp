#pragma once

#include <string>

#include "../../include/Events/Event.h"
#include "../../include/Events/EventListener.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	EventListener::EventListener() : m_enabled(true) {}

	void EventListener::ProcessEvent(Event& e) { ENGINE_TRACE_D("Process event for EventListener"); };

	const bool EventListener::IsEnabled() const
	{
		return m_enabled;
	}
}