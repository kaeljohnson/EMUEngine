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