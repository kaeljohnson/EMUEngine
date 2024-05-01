#pragma once

#include <string>

#include "../../include/Events/EventListener.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Scenes/SceneObject.h"
#include "../../include/CallbackSystem/CallbackSystem.h"

namespace Engine
{
	EventListener::EventListener(std::string name) : ptrICallbackSystem(ICallbackSystem::GetInstance()), m_enabled(true), IsAttachedToApp(false), m_name(name) {}

	void EventListener::ProcessEvent(Event& e) { ENGINE_TRACE_D("Process event for EventListener {}", m_name); };
}