#pragma once

#include <string>

#include "EventListener.h"

namespace Engine
{
	// Event Listener Stack.

	class EventListenerStack
	{
	public:
		static const size_t MAX_LISTENERS = 10;
		size_t m_eventListenerCount = 0;
		EventListener* m_eventListeners[MAX_LISTENERS];

	public:
		EventListenerStack();
		~EventListenerStack() = default;

		EventListener** begin() { return m_eventListeners; }
		EventListener** end() { return m_eventListeners + m_eventListenerCount; }

		const size_t size() const;

		// Pushes a listener to the stack.
		void push(EventListener* listener);

		// Pops a listener from the stack.
		void pop(EventListener* listener);

		// Pops the top listener from the stack.
		void pop();
	};
}