#pragma once

namespace Engine
{
	// Custom event types to hide the SDL2 event types from the user.
	enum EventType
	{
		QUIT = 0,
		TOGGLE_FULLSCREEN, RESIZE_WINDOW,

		// Number key down events
		ONE_KEY_DOWN, TWO_KEY_DOWN, THREE_KEY_DOWN, FOUR_KEY_DOWN, 
		FIVE_KEY_DOWN, SIX_KEY_DOWN, SEVEN_KEY_DOWN, EIGHT_KEY_DOWN, 
		NINE_KEY_DOWN, ZERO_KEY_DOWN,

		// Number key up events
		ONE_KEY_UP, TWO_KEY_UP, THREE_KEY_UP, FOUR_KEY_UP,
		FIVE_KEY_UP, SIX_KEY_UP, SEVEN_KEY_UP, EIGHT_KEY_UP,
		NINE_KEY_UP, ZERO_KEY_UP,

		// Letter key down events
		A_KEY_DOWN, B_KEY_DOWN, C_KEY_DOWN, D_KEY_DOWN, E_KEY_DOWN, F_KEY_DOWN, 
		G_KEY_DOWN, H_KEY_DOWN, I_KEY_DOWN, J_KEY_DOWN, K_KEY_DOWN, L_KEY_DOWN, 
		M_KEY_DOWN, N_KEY_DOWN, O_KEY_DOWN, P_KEY_DOWN, Q_KEY_DOWN, R_KEY_DOWN, 
		S_KEY_DOWN, T_KEY_DOWN, U_KEY_DOWN, V_KEY_DOWN, W_KEY_DOWN, X_KEY_DOWN, 
		Y_KEY_DOWN, Z_KEY_DOWN,

		// Letter key up events
		A_KEY_UP, B_KEY_UP, C_KEY_UP, D_KEY_UP, E_KEY_UP, F_KEY_UP,
		G_KEY_UP, H_KEY_UP, I_KEY_UP, J_KEY_UP, K_KEY_UP, L_KEY_UP,
		M_KEY_UP, N_KEY_UP, O_KEY_UP, P_KEY_UP, Q_KEY_UP, R_KEY_UP,
		S_KEY_UP, T_KEY_UP, U_KEY_UP, V_KEY_UP, W_KEY_UP, X_KEY_UP,
		Y_KEY_UP, Z_KEY_UP,
		
		// Arrow key down events
		LEFT_ARROW_KEY_DOWN, RIGHT_ARROW_KEY_DOWN, UP_ARROW_KEY_DOWN, DOWN_ARROW_KEY_DOWN,

		// Arrow key up events
		LEFT_ARROW_KEY_UP, RIGHT_ARROW_KEY_UP, UP_ARROW_KEY_UP, DOWN_ARROW_KEY_UP,

		// Special key down events
		CAPS_LOCK_KEY_DOWN, LEFT_SHIFT_KEY_DOWN, RIGHT_SHIFT_KEY_DOWN, 
		SPACE_KEY_DOWN, ENTER_KEY_DOWN, EQUALS_KEY_DOWN,
		BACKSPACE_KEY_DOWN, TAB_KEY_DOWN, ESCAPE_KEY_DOWN, MINUS_KEY_DOWN,

		// Special key up events
		CAPS_LOCK_KEY_UP, LEFT_SHIFT_KEY_UP, RIGHT_SHIFT_KEY_UP,
		SPACE_KEY_UP, ENTER_KEY_UP, EQUALS_KEY_UP,
		BACKSPACE_KEY_UP, TAB_KEY_UP, ESCAPE_KEY_UP, MINUS_KEY_UP,

		// Mouse button down events
		LEFT_MOUSE_BUTTON_DOWN, RIGHT_MOUSE_BUTTON_DOWN, MIDDLE_MOUSE_BUTTON_DOWN,

		// Mouse button up events
		LEFT_MOUSE_BUTTON_UP, RIGHT_MOUSE_BUTTON_UP, MIDDLE_MOUSE_BUTTON_UP,

		// Mouse movement events
		MOUSE_MOVE, MOUSE_DRAGGED,

		// Mouse wheel events
		MOUSE_WHEEL_MOVED,

		// Engine processig events
		ENGINE_NOT_INITIALIZED, ENGINE_INITIALIZED, ENGINE_SHUTDOWN, ENGINE_ERROR
	};


	
	// Custom event class to hide the SDL2 event from the user.
	class Event
	{	
	public:
		const EventType Type;
		const int X_POS;
		const int Y_POS;
		bool Handled;

		Event(const EventType);
		Event(const EventType actionType, const int xPos, const int yPos);
	};
}
 