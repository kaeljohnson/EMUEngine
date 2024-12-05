#pragma once
#include <unordered_map>

#include "IOEvent.h"

#include "../Core.h"


namespace Engine
{
	using KeyStatesMap = std::unordered_map<IOEventType, bool>;

	struct KeyStates
	{
		EMU_API static const KeyStatesMap& GetKeyStates() { return m_keyStates; }

	protected:
		static KeyStatesMap m_keyStates;

		friend class IOEventDispatcher;
		friend class IOEventSystem;
	};
}