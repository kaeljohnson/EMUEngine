#pragma once

#include <vector>
#include <string>

#include "EventListener.h"

namespace Engine
{

	// Event Listener Stack.

	class EventListenerStack
	{
	private:
		std::vector<EventListener*> m_eventListeners;

	public:
		const size_t size() const;

		EventListenerStack();
		EventListenerStack(std::vector<EventListener*> eventListeners);
		~EventListenerStack();

		std::vector<EventListener*>::iterator begin() { return m_eventListeners.begin(); }
		std::vector<EventListener*>::iterator end() { return m_eventListeners.end(); }

		// Pushes a listener to the stack.
		void push(EventListener* listener);

		// Pops a listener from the stack.
		void pop(EventListener* listener);

		// Pops the top listener from the stack.
		void pop();
	};
}