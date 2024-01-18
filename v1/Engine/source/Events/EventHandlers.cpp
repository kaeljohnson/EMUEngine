#pragma once

#include "../../include/Events/EventHandlers.h"

namespace Engine
{
	// custom event handlers defined below.

	void handleQuitEvent()
	{
		printf("handle quit event");
	}

	void handleKeyDownEvent()
	{
		printf("handle key down event.");
	}

	void handleKeyUpEvent()
	{
		printf("handle key up event.");
	}

	void handleMouseMoveEvent()
	{
		printf("handle mouse move event.");
	}

	void handleMouseButtonDownEvent()
	{
		printf("handle mouse button down event.");
	}

	void handleMouseButtonUpEvent()
	{
		printf("handle mouse button up event.");
	}

	void handleMouseScrollEvent()
	{
		printf("handle mouse scroll event.");
	}

	void handleWindowEvent()
	{
		printf("handle window event.");
	}
	
	/*void defKeydownHandler(keydown_handler_t);
	void defKeyUpHandler(keyup_handler_t);
	void defMouseMoveHandler(mouse_move_handler_t);
	void defMouseButtonDownHandler(mouse_button_down_event_t);
	void defMouseButtonUpHandler(mouse_button_up_event_t);
	void defMouseScrollEvent(mouse_scroll_event_t);*/

	EventHandlers::EventHandlers()
		: quit(handleQuitEvent),
		keydown(handleKeyUpEvent),
		keyup(handleKeyUpEvent),
		mouseMove(handleMouseMoveEvent),
		mouseButtonDown(handleMouseButtonDownEvent),
		mouseButtonUp(handleMouseButtonUpEvent),
		mouseScroll(handleMouseScrollEvent),
		windowEvent(handleWindowEvent)
	{}
}