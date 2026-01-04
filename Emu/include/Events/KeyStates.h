#pragma once

#include "../Includes.h"
#include "IOEvent.h"

namespace Engine
{
	using KeyStatesMap = std::unordered_map<IOEventType, bool>;

	/**
	 * @brief Manages the states of keys (pressed/released) for IO events.
	 */
	struct KeyStates
	{
		static const KeyStatesMap& GetKeyStates() { return m_keyStates; }

	protected:
		static KeyStatesMap m_keyStates;

		friend class IOEventDispatcher;
		friend class IOEventSystem;
	};
}