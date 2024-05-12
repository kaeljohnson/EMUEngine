#pragma once

#include "../../include/Events/EventListenerStack.h"
#include "../../include/Events/EventListenerStack.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	EventListenerStack::EventListenerStack()
	{
		for (int i = 0; i < MAX_LISTENERS; i++)
		{
			m_eventListeners[i] = nullptr;
		}
	}

	const size_t EventListenerStack::size() const { return m_eventListenerCount; }

	void EventListenerStack::push(EventListener* eventListener)
	{
		if (eventListener == nullptr)
		{
			ENGINE_CRITICAL_D("EventListener is nullptr. Cannot push nullptr to the EventListener stack.");
			return;
		}

		// Check if the event listener already exists in the array
		for (int i = 0; i < m_eventListenerCount; i++)
		{
			if (m_eventListeners[i] == eventListener)
			{
				ENGINE_CRITICAL_D("EventListener already exists in the EventListener stack!");
				return;
			}
		}

		// Check if there's room in the array
		if (m_eventListenerCount >= MAX_LISTENERS)
		{
			ENGINE_CRITICAL_D("EventListener stack is full. Cannot add more event listeners.");
			return;
		}

		// Add the event listener to the end of the array
		m_eventListeners[m_eventListenerCount] = eventListener;
		m_eventListenerCount++;
	}

	void EventListenerStack::pop(EventListener* eventListener)
	{
		if (eventListener == nullptr)
		{
			ENGINE_CRITICAL_D("EventListener is nullptr. Cannot pop nullptr from the EventListener stack.");
			return;
		}

		// Find the event listener in the array
		for (int i = 0; i < m_eventListenerCount; i++)
		{
			if (m_eventListeners[i] == eventListener)
			{
				// Remove the event listener from the array by moving all elements after it one step to the left
				for (int j = i; j < m_eventListenerCount - 1; j++)
				{
					m_eventListeners[j] = m_eventListeners[j + 1];
				}

				// Decrease the count of event listeners
				m_eventListenerCount--;

				break;
			}
		}
	}

	void EventListenerStack::pop()
	{
		if (m_eventListenerCount == 0)
		{
			ENGINE_CRITICAL_D("EventListener stack is empty. Cannot pop EventListener from an empty EventListener stack.");
			return;
		}

		// Decrease the count of event listeners
		m_eventListenerCount--;
	}
}