#pragma once

#include <stdio.h>


namespace Engine
{
	class EventHandlers
	{
		typedef void (*quit_handler_t)(void);
		typedef void (*keydown_handler_t)(void);
		typedef void (*keyup_handler_t)(void);
		typedef void (*mouse_move_handler_t)(void);
		typedef void (*mouse_button_down_event_t)(void);
		typedef void (*mouse_button_up_event_t)(void);
		typedef void (*mouse_scroll_event_t)(void);
		typedef void (*window_event_t)(void);

	public:
		quit_handler_t            quit;
		keydown_handler_t         keydown;
		keyup_handler_t           keyup;
		mouse_move_handler_t      mouseMove;
		mouse_button_down_event_t mouseButtonDown;
		mouse_button_up_event_t   mouseButtonUp;
		mouse_scroll_event_t      mouseScroll;
		window_event_t            windowEvent;

		EventHandlers(/*User should set their custom event handlers somehow.*/);
	};
}