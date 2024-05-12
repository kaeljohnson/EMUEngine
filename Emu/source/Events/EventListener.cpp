#pragma once

#include <string>

#include "../../include/Events/EventListener.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Scenes/SceneObject.h"
#include "../../include/CallbackSystem/CallbackSystem.h"

namespace Engine
{
	EventListener::EventListener() : ptrICallbackSystem(ICallbackSystem::GetInstance()), m_enabled(true) {}

	void EventListener::ProcessEvent(Event& e) { ENGINE_TRACE_D("Process event for EventListener"); };

	const bool EventListener::IsEnabled() const
	{
		return m_enabled;
	}
}