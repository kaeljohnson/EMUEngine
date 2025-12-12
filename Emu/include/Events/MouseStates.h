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
		EMU_API static const Math2D::Point2D<int>& GetMousePosition() { return m_mousePosition; }
		EMU_API static const Math2D::Point2D<int>& GetScrollDirection()  { return m_scrollDirection; }

	protected:
		static MouseStatesMap m_mouseStates;
		static Math2D::Point2D<int> m_mousePosition;
		static Math2D::Point2D<int> m_scrollDirection;

		friend class IOEventDispatcher;
		friend class IOEventSystem;
	};
}