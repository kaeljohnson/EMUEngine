#pragma once
#include <unordered_map>

#include "Event.h"

#include "../Core.h"


namespace Engine
{
	using KeyStatesMap = std::unordered_map<EventType, bool>;

	struct KeyStates
	{
		EMU_API static const KeyStatesMap& GetKeyStates() { return m_keyStates; }

	protected:
		static KeyStatesMap m_keyStates;

		friend class EventDispatcher;
		friend class EventManager;
	};
}