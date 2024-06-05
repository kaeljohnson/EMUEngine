#pragma once

#include "../EngineConstants.h"
#include "EventListener.h"

namespace Engine
{
	// Event Listener Stack.

	class EventListenerStack
	{
	private:	
		size_t m_eventListenerCount = 0;
		EventListener* m_eventListeners[MAX_LISTENERS];

	public:
		EventListenerStack();
		~EventListenerStack() = default;

		const size_t Size() const;
		void Push(EventListener* listener);
		void Pop(EventListener* listener);
		void Pop();

		EventListener** const begin() { return m_eventListeners; }
		EventListener** const end() { return m_eventListeners + m_eventListenerCount; }
	};
}