#pragma once

#include <SDL.h>
#include <queue>

#include "../../include/Events/EventManager.h"
#include "../../include/Events/Event.h"

namespace Engine
{
	EventManager::EventManager(std::queue<Event>& eventQ)
		: refEventQ(eventQ) {}

	void EventManager::handleEvents()
	{
		/* 
			Poll for SDLevents and dispatch them to their respective event types.
			The dispatch functions will push the event to the event queue based 
			on their sub type. Key down and key up events are handled differently, 
			as they have to be checked for their key code. Key held events are 
			handled separately, based on how the user implements respective event 
			handling. The reason for this is that the SDL_KEYDOWN event has a delay 
			before it starts repeating when key is held, making it hard to handle.
		*/

		SDL_Event e;

		while (SDL_PollEvent(&e))
		{ 
			// If the event is a keydown event and the key is repeating, we don't want to handle it.
			if (e.type == SDL_KEYDOWN && e.key.repeat == 1) return;

			switch (e.type)
			{
			case (SDL_QUIT): dispatchQuitEvent(); break;
			case (SDL_WINDOWEVENT): dispatchWindowEvent(e.window); break;
			case (SDL_KEYDOWN): dispatchKeydownEvent(e.key.keysym.sym); break;
			case (SDL_KEYUP): dispatchKeyupEvent(e.key.keysym.sym); break;
			case (SDL_MOUSEBUTTONDOWN): dispatchMouseButtonDownEvent(e.button); break;
			case (SDL_MOUSEBUTTONUP): dispatchMouseButtonUpEvent(e.button); break;
			case (SDL_MOUSEMOTION): dispatchMouseMoveEvent(e.motion); break;
			case (SDL_MOUSEWHEEL): dispatchMouseScrollEvent(e.wheel); break;
			default: break;
			}
		}
	}

	void EventManager::dispatchQuitEvent()
	{
		refEventQ.push({QUIT});
	}

	void EventManager::dispatchWindowEvent(SDL_WindowEvent& windowEvent)
	{
		switch (windowEvent.type)
		{
			case (SDL_WINDOWEVENT_RESIZED): refEventQ.push({ RESIZE_WINDOW, windowEvent.data1, windowEvent.data2 }); break;
			default: break;
		}
	}

	void EventManager::dispatchKeydownEvent(SDL_Keycode& keyCode)
	{
		switch (keyCode)
		{
			case (SDLK_ESCAPE): refEventQ.push({ESCAPE_KEY_DOWN}); break;
			case (SDLK_EQUALS): refEventQ.push({EQUALS_KEY_DOWN}); break;
			case (SDLK_0): refEventQ.push({ZERO_KEY_DOWN}); break;
			case (SDLK_1): refEventQ.push({ONE_KEY_DOWN}); break;
			case (SDLK_2): refEventQ.push({TWO_KEY_DOWN}); break;
			case (SDLK_3): refEventQ.push({THREE_KEY_DOWN}); break;
			case (SDLK_4): refEventQ.push({FOUR_KEY_DOWN}); break;
			case (SDLK_5): refEventQ.push({FIVE_KEY_DOWN}); break;
			case (SDLK_6): refEventQ.push({SIX_KEY_DOWN}); break;
			case (SDLK_7): refEventQ.push({SEVEN_KEY_DOWN}); break;
			case (SDLK_8): refEventQ.push({EIGHT_KEY_DOWN}); break;
			case (SDLK_9): refEventQ.push({NINE_KEY_DOWN}); break;
			case (SDLK_q): refEventQ.push({Q_KEY_DOWN}); break;
			case (SDLK_w): refEventQ.push({W_KEY_DOWN}); break;
			case (SDLK_e): refEventQ.push({E_KEY_DOWN}); break;
			case (SDLK_r): refEventQ.push({R_KEY_DOWN}); break;
			case (SDLK_t): refEventQ.push({T_KEY_DOWN}); break;
			case (SDLK_y): refEventQ.push({Y_KEY_DOWN}); break;
			case (SDLK_u): refEventQ.push({U_KEY_DOWN}); break;
			case (SDLK_i): refEventQ.push({I_KEY_DOWN}); break;
			case (SDLK_o): refEventQ.push({O_KEY_DOWN}); break;
			case (SDLK_p): refEventQ.push({P_KEY_DOWN}); break;
			case (SDLK_a): refEventQ.push({A_KEY_DOWN}); break;
			case (SDLK_s): refEventQ.push({S_KEY_DOWN}); break;
			case (SDLK_d): refEventQ.push({D_KEY_DOWN}); break;
			case (SDLK_f): refEventQ.push({F_KEY_DOWN}); break;
			case (SDLK_g): refEventQ.push({G_KEY_DOWN}); break;
			case (SDLK_h): refEventQ.push({H_KEY_DOWN}); break;
			case (SDLK_j): refEventQ.push({J_KEY_DOWN}); break;
			case (SDLK_k): refEventQ.push({K_KEY_DOWN}); break;
			case (SDLK_l): refEventQ.push({L_KEY_DOWN}); break;
			case (SDLK_z): refEventQ.push({Z_KEY_DOWN}); break;
			case (SDLK_x): refEventQ.push({X_KEY_DOWN}); break;
			case (SDLK_c): refEventQ.push({C_KEY_DOWN}); break;
			case (SDLK_v): refEventQ.push({V_KEY_DOWN}); break;
			case (SDLK_b): refEventQ.push({B_KEY_DOWN}); break;
			case (SDLK_n): refEventQ.push({N_KEY_DOWN}); break;
			case (SDLK_m): refEventQ.push({M_KEY_DOWN}); break;
			case (SDLK_TAB): refEventQ.push({TAB_KEY_DOWN}); break;
			case (SDLK_CAPSLOCK): refEventQ.push({CAPS_LOCK_KEY_DOWN}); break;
			case (SDLK_LSHIFT): refEventQ.push({LEFT_SHIFT_KEY_DOWN}); break;
			case (SDLK_RSHIFT): refEventQ.push({RIGHT_SHIFT_KEY_DOWN}); break;
			case (SDLK_RETURN): refEventQ.push({ENTER_KEY_DOWN}); break;
			case (SDLK_LEFT): refEventQ.push({LEFT_ARROW_KEY_DOWN}); break;
			case (SDLK_UP): refEventQ.push({UP_ARROW_KEY_DOWN}); break;
			case (SDLK_RIGHT): refEventQ.push({RIGHT_ARROW_KEY_DOWN}); break;
			case (SDLK_DOWN): refEventQ.push({DOWN_ARROW_KEY_DOWN}); break;
			case (SDLK_SPACE): refEventQ.push({SPACE_KEY_DOWN}); break;
			case (SDLK_MINUS): refEventQ.push({MINUS_KEY_DOWN}); break;
			case (SDLK_BACKSPACE): refEventQ.push({BACKSPACE_KEY_DOWN}); break;
			default: break;

		}
	}

	void EventManager::dispatchKeyupEvent(SDL_Keycode& keyCode)
	{
		switch (keyCode)
		{
			case (SDLK_ESCAPE): refEventQ.push({ESCAPE_KEY_UP}); break;
			case (SDLK_EQUALS): refEventQ.push({EQUALS_KEY_UP}); break;
			case (SDLK_0): refEventQ.push({ZERO_KEY_UP}); break;
			case (SDLK_1): refEventQ.push({ONE_KEY_UP}); break;
			case (SDLK_2): refEventQ.push({TWO_KEY_UP}); break;
			case (SDLK_3): refEventQ.push({THREE_KEY_UP}); break;
			case (SDLK_4): refEventQ.push({FOUR_KEY_UP}); break;
			case (SDLK_5): refEventQ.push({FIVE_KEY_UP}); break;
			case (SDLK_6): refEventQ.push({SIX_KEY_UP}); break;
			case (SDLK_7): refEventQ.push({SEVEN_KEY_UP}); break;
			case (SDLK_8): refEventQ.push({EIGHT_KEY_UP}); break;
			case (SDLK_9): refEventQ.push({NINE_KEY_UP}); break;
			case (SDLK_q): refEventQ.push({Q_KEY_UP}); break;
			case (SDLK_w): refEventQ.push({W_KEY_UP}); break;
			case (SDLK_e): refEventQ.push({E_KEY_UP}); break;
			case (SDLK_r): refEventQ.push({R_KEY_UP}); break;
			case (SDLK_t): refEventQ.push({T_KEY_UP}); break;
			case (SDLK_y): refEventQ.push({Y_KEY_UP}); break;
			case (SDLK_u): refEventQ.push({U_KEY_UP}); break;
			case (SDLK_i): refEventQ.push({I_KEY_UP}); break;
			case (SDLK_o): refEventQ.push({O_KEY_UP}); break;
			case (SDLK_p): refEventQ.push({P_KEY_UP}); break;
			case (SDLK_a): refEventQ.push({A_KEY_UP}); break;
			case (SDLK_s): refEventQ.push({S_KEY_UP}); break;
			case (SDLK_d): refEventQ.push({D_KEY_UP}); break;
			case (SDLK_f): refEventQ.push({F_KEY_UP}); break;
			case (SDLK_g): refEventQ.push({G_KEY_UP}); break;
			case (SDLK_h): refEventQ.push({H_KEY_UP}); break;
			case (SDLK_j): refEventQ.push({J_KEY_UP}); break;
			case (SDLK_k): refEventQ.push({K_KEY_UP}); break;
			case (SDLK_l): refEventQ.push({L_KEY_UP}); break;
			case (SDLK_z): refEventQ.push({Z_KEY_UP}); break;
			case (SDLK_x): refEventQ.push({X_KEY_UP}); break;
			case (SDLK_c): refEventQ.push({C_KEY_UP}); break;
			case (SDLK_v): refEventQ.push({V_KEY_UP}); break;
			case (SDLK_b): refEventQ.push({B_KEY_UP}); break;
			case (SDLK_n): refEventQ.push({N_KEY_UP}); break;
			case (SDLK_m): refEventQ.push({M_KEY_UP}); break;
			case (SDLK_TAB): refEventQ.push({TAB_KEY_UP}); break;
			case (SDLK_CAPSLOCK): refEventQ.push({CAPS_LOCK_KEY_UP}); break;
			case (SDLK_LSHIFT): refEventQ.push({LEFT_SHIFT_KEY_UP}); break;
			case (SDLK_RSHIFT): refEventQ.push({RIGHT_SHIFT_KEY_UP}); break;
			case (SDLK_RETURN): refEventQ.push({ENTER_KEY_UP}); break;
			case (SDLK_LEFT): refEventQ.push({LEFT_ARROW_KEY_UP}); break;
			case (SDLK_UP): refEventQ.push({UP_ARROW_KEY_UP}); break;
			case (SDLK_RIGHT): refEventQ.push({RIGHT_ARROW_KEY_UP}); break;
			case (SDLK_DOWN): refEventQ.push({DOWN_ARROW_KEY_UP}); break;
			case (SDLK_SPACE): refEventQ.push({SPACE_KEY_UP}); break;
			case (SDLK_MINUS): refEventQ.push({MINUS_KEY_UP}); break;
			case (SDLK_BACKSPACE): refEventQ.push({BACKSPACE_KEY_UP}); break;
			default: break;
		}
	}
	void EventManager::dispatchMouseMoveEvent(SDL_MouseMotionEvent& mouseMotion)
	{
		refEventQ.push({ MOUSE_MOVED, mouseMotion.x, mouseMotion.y });
	}
	void EventManager::dispatchMouseButtonDownEvent(SDL_MouseButtonEvent& mouseButtonEvent)
	{
		switch (mouseButtonEvent.button)
		{
			case (SDL_BUTTON_LEFT): refEventQ.push({ LEFT_MOUSE_BUTTON_DOWN }); break;
			case (SDL_BUTTON_MIDDLE): refEventQ.push({ MIDDLE_MOUSE_BUTTON_DOWN }); break;
			case (SDL_BUTTON_RIGHT): refEventQ.push({ RIGHT_MOUSE_BUTTON_DOWN }); break;
			default: break;
		}
	}
	void EventManager::dispatchMouseButtonUpEvent(SDL_MouseButtonEvent& mouseButtonEvent)
	{
		switch (mouseButtonEvent.button)
		{
			case (SDL_BUTTON_LEFT): refEventQ.push({ LEFT_MOUSE_BUTTON_UP }); break;
			case (SDL_BUTTON_MIDDLE): refEventQ.push({ MIDDLE_MOUSE_BUTTON_UP }); break;
			case (SDL_BUTTON_RIGHT): refEventQ.push({ RIGHT_MOUSE_BUTTON_UP }); break;
			default: break;
		}
	}
	void EventManager::dispatchMouseScrollEvent(SDL_MouseWheelEvent& mouseWheelEvent)
	{
		switch (mouseWheelEvent.direction)
		{
			case (SDL_MOUSEWHEEL_NORMAL): refEventQ.push({ MOUSE_WHEEL_MOVED, mouseWheelEvent.x, mouseWheelEvent.y }); break;
			default: break;
		}
	}
}