#pragma once

#include <vector>

#include "../../include/Events/EventListenerStack.h"
#include "../../include/Events/EventListenerStack.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	EventListenerStack::EventListenerStack() {}
	EventListenerStack::EventListenerStack(std::vector<EventListener*> EventListeners) : m_eventListeners(EventListeners) {}
	EventListenerStack::~EventListenerStack()
	{
	}

	const size_t EventListenerStack::size() const { return m_eventListeners.size(); }

	void EventListenerStack::push(EventListener* EventListener) 
	{
		if (EventListener == nullptr)
		{
			ENGINE_CRITICAL_D("EventListener is nullptr. Cannot push nullptr to the EventListener stack.");
			return;
		}

		for (auto it = m_eventListeners.begin(); it != m_eventListeners.end(); ++it)
		{
			if ((*it)->m_name == EventListener->m_name)
			{
				ENGINE_CRITICAL_D("EventListener with name: {} already exists in the EventListener stack!", EventListener->m_name);
				return;
			}
		}		

		// Add the EventListener to the EventListener stack.
		m_eventListeners.push_back(EventListener);
	}

	void EventListenerStack::pop(EventListener* EventListener) 
	{
		if (EventListener == nullptr)
		{
			ENGINE_CRITICAL_D("EventListener is nullptr. Cannot pop nullptr from the EventListener stack.");
			return;
		}

		for (auto it = m_eventListeners.begin(); it != m_eventListeners.end(); it++)
		{
			if ((*it)->m_name == EventListener->m_name)
			{

				(*it)->IsAttachedToScene = false;

				// Remove the EventListener from the EventListener stack.
				m_eventListeners.erase(it);
				break;
			}
		}
	}

	void EventListenerStack::pop() 
	{
		if (m_eventListeners.empty())
		{
			ENGINE_CRITICAL_D("EventListener stack is empty. Cannot pop EventListener from an empty EventListener stack.");
			return;
		}

		m_eventListeners.back()->IsAttachedToScene = false;

		// Remove the EventListener from the EventListener stack.
		m_eventListeners.pop_back();
	}
}