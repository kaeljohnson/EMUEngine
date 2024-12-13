#pragma once

#include "../Core.h"
#include "../MathUtil.h"
#include "../Includes.h"
#include "IOEvent.h"

namespace Engine
{
	using MouseStatesMap = std::unordered_map<IOEventType, bool>;

	struct MouseStates
	{
		EMU_API static const MouseStatesMap& GetMouseStates() { return m_mouseStates; }
		EMU_API static const Vector2D<int>& GetMousePosition() { return m_mousePosition; }
		EMU_API static const Vector2D<int>& GetScrollDirection()  { return m_scrollDirection; }

	protected:
		static MouseStatesMap m_mouseStates;
		static Vector2D<int> m_mousePosition;
		static Vector2D<int> m_scrollDirection;

		friend class IOEventDispatcher;
		friend class IOEventSystem;
	};
}