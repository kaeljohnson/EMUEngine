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
		/**
		* @brief Retrieves the current states of keys.
		* * @return A constant reference to a map of key states.
		*/
		static const KeyStatesMap& GetKeyStates() { return m_keyStates; }

	protected:
		static KeyStatesMap m_keyStates; /// Map storing the states of keys.

		friend class IOEventDispatcher;
		friend class IOEventSystem;
	};
}