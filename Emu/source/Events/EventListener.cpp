#pragma once

#include <string>

#include "../../include/Events/EventListener.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Scenes/SceneObject.h"
#include "../../include/CallbackSystem/CallbackSystem.h"

namespace Engine
{
	EventListener::EventListener(std::string name) : ptrICallbackSystem(ICallbackSystem::GetInstance()), m_enabled(true), m_attachedToApp(false), m_name(name) {}

	void EventListener::ProcessEvent(Event& e) { ENGINE_TRACE_D("Process event for EventListener {}", m_name); };

	const std::string EventListener::GetName() const
	{
		return m_name;
	}

	const bool EventListener::IsEnabled() const
	{
		return m_enabled;
	}

	const bool EventListener::IsAttached() const
	{
		return m_attachedToApp;
	}

	void EventListener::SetAttached(bool attached)
	{
		m_attachedToApp = attached;
	}
}