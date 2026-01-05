#pragma once

#include "../MathUtil.h"
#include "../Includes.h"
#include "IOEvent.h"

namespace Engine
{
	using MouseStatesMap = std::unordered_map<IOEventType, bool>;

	/**
	 * @brief Manages the states of mouse buttons (pressed/released) and mouse position for IO events.
	 */
	struct MouseStates
	{
		/**
		* @brief Retrieves the current states of mouse buttons.
		* * @return A constant reference to a map of mouse button states.
		*/
		static const MouseStatesMap& GetMouseStates() { return m_mouseStates; }

		/**
		* @brief Retrieves the current mouse position.
		* * @return A constant reference to the mouse position.
		*/
		static const Math2D::Point2D<int>& GetMousePosition() { return m_mousePosition; }

		/**
		* @brief Retrieves the current scroll direction.
		* * @return A constant reference to the scroll direction.
		*/
		static const Math2D::Point2D<int>& GetScrollDirection()  { return m_scrollDirection; }

	protected:
		static MouseStatesMap m_mouseStates;			/// Map storing the states of mouse buttons.
		static Math2D::Point2D<int> m_mousePosition;	/// Current mouse position.
		static Math2D::Point2D<int> m_scrollDirection;	/// Current scroll direction.

		friend class IOEventDispatcher;
		friend class IOEventSystem;
	};
}