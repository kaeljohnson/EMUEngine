#pragma once

#include <queue>

#include "../../include/SDLWrapper/SDLWrapper.h"
#include "../../include/Events/EventManager.h"
#include "../../include/Events/Event.h"

namespace Engine
{
    EventManager* EventManager::instance = nullptr;

    EventManager::EventManager() {
        // Initialize all key down states to false
        keyStates[ESCAPE_KEY_DOWN] = false;
        keyStates[EQUALS_KEY_DOWN] = false;
        keyStates[ZERO_KEY_DOWN] = false;
        keyStates[ONE_KEY_DOWN] = false;
        keyStates[TWO_KEY_DOWN] = false;
        keyStates[THREE_KEY_DOWN] = false;
        keyStates[FOUR_KEY_DOWN] = false;
        keyStates[FIVE_KEY_DOWN] = false;
        keyStates[SIX_KEY_DOWN] = false;
        keyStates[SEVEN_KEY_DOWN] = false;
        keyStates[EIGHT_KEY_DOWN] = false;
        keyStates[NINE_KEY_DOWN] = false;
        keyStates[Q_KEY_DOWN] = false;
        keyStates[W_KEY_DOWN] = false;
        keyStates[E_KEY_DOWN] = false;
        keyStates[R_KEY_DOWN] = false;
        keyStates[T_KEY_DOWN] = false;
        keyStates[Y_KEY_DOWN] = false;
        keyStates[U_KEY_DOWN] = false;
        keyStates[I_KEY_DOWN] = false;
        keyStates[O_KEY_DOWN] = false;
        keyStates[P_KEY_DOWN] = false;
        keyStates[A_KEY_DOWN] = false;
        keyStates[S_KEY_DOWN] = false;
        keyStates[D_KEY_DOWN] = false;
        keyStates[F_KEY_DOWN] = false;
        keyStates[G_KEY_DOWN] = false;
        keyStates[H_KEY_DOWN] = false;
        keyStates[J_KEY_DOWN] = false;
        keyStates[K_KEY_DOWN] = false;
        keyStates[L_KEY_DOWN] = false;
        keyStates[Z_KEY_DOWN] = false;
        keyStates[X_KEY_DOWN] = false;
        keyStates[C_KEY_DOWN] = false;
        keyStates[V_KEY_DOWN] = false;
        keyStates[B_KEY_DOWN] = false;
        keyStates[N_KEY_DOWN] = false;
        keyStates[M_KEY_DOWN] = false;
        keyStates[TAB_KEY_DOWN] = false;
        keyStates[CAPS_LOCK_KEY_DOWN] = false;
        keyStates[LEFT_SHIFT_KEY_DOWN] = false;
        keyStates[RIGHT_SHIFT_KEY_DOWN] = false;
        keyStates[ENTER_KEY_DOWN] = false;
        keyStates[LEFT_ARROW_KEY_DOWN] = false;
        keyStates[UP_ARROW_KEY_DOWN] = false;
        keyStates[RIGHT_ARROW_KEY_DOWN] = false;
        keyStates[DOWN_ARROW_KEY_DOWN] = false;
        keyStates[SPACE_KEY_DOWN] = false;
        keyStates[MINUS_KEY_DOWN] = false;
        keyStates[BACKSPACE_KEY_DOWN] = false;

        // Initialize all key up states to true
        keyStates[ESCAPE_KEY_UP] = true;
        keyStates[EQUALS_KEY_UP] = true;
        keyStates[ZERO_KEY_UP] = true;
        keyStates[ONE_KEY_UP] = true;
        keyStates[TWO_KEY_UP] = true;
        keyStates[THREE_KEY_UP] = true;
        keyStates[FOUR_KEY_UP] = true;
        keyStates[FIVE_KEY_UP] = true;
        keyStates[SIX_KEY_UP] = true;
        keyStates[SEVEN_KEY_UP] = true;
        keyStates[EIGHT_KEY_UP] = true;
        keyStates[NINE_KEY_UP] = true;
        keyStates[Q_KEY_UP] = true;
        keyStates[W_KEY_UP] = true;
        keyStates[E_KEY_UP] = true;
        keyStates[R_KEY_UP] = true;
        keyStates[T_KEY_UP] = true;
        keyStates[Y_KEY_UP] = true;
        keyStates[U_KEY_UP] = true;
        keyStates[I_KEY_UP] = true;
        keyStates[O_KEY_UP] = true;
        keyStates[P_KEY_UP] = true;
        keyStates[A_KEY_UP] = true;
        keyStates[S_KEY_UP] = true;
        keyStates[D_KEY_UP] = true;
        keyStates[F_KEY_UP] = true;
        keyStates[G_KEY_UP] = true;
        keyStates[H_KEY_UP] = true;
        keyStates[J_KEY_UP] = true;
        keyStates[K_KEY_UP] = true;
        keyStates[L_KEY_UP] = true;
        keyStates[Z_KEY_UP] = true;
        keyStates[X_KEY_UP] = true;
        keyStates[C_KEY_UP] = true;
        keyStates[V_KEY_UP] = true;
        keyStates[B_KEY_UP] = true;
        keyStates[N_KEY_UP] = true;
        keyStates[M_KEY_UP] = true;
        keyStates[TAB_KEY_UP] = true;
        keyStates[CAPS_LOCK_KEY_UP] = true;
        keyStates[LEFT_SHIFT_KEY_UP] = true;
        keyStates[RIGHT_SHIFT_KEY_UP] = true;
        keyStates[ENTER_KEY_UP] = true;
        keyStates[LEFT_ARROW_KEY_UP] = true;
        keyStates[UP_ARROW_KEY_UP] = true;
        keyStates[RIGHT_ARROW_KEY_UP] = true;
        keyStates[DOWN_ARROW_KEY_UP] = true;
        keyStates[SPACE_KEY_UP] = true;
        keyStates[MINUS_KEY_UP] = true;
        keyStates[BACKSPACE_KEY_UP] = true;
    }

	EventManager* EventManager::GetInstance()
	{
        if (instance == nullptr)
        {
			instance = new EventManager();
		}
		return instance;
	}

    const std::unordered_map<EventType, bool>& EventManager::GetKeyStates() const
	{
		return keyStates;
	}

	void EventManager::HandleEvents()
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

		SDLEvent e;

		while (SDL_POLL_EVENT(&e))
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
        eventQ.push({ QUIT });
    }

    void EventManager::dispatchWindowEvent(SDL_WindowEvent& windowEvent)
    {
        switch (windowEvent.event)
        {
        case (SDL_WINDOWEVENT_RESIZED): eventQ.push({ RESIZE_WINDOW, windowEvent.data1, windowEvent.data2 }); break;
        default: break;
        }
    }

    void EventManager::dispatchKeydownEvent(SDL_Keycode& keyCode)
    {
        switch (keyCode)
        {
        case (SDLK_ESCAPE): keyStates[ESCAPE_KEY_DOWN] = true; keyStates[ESCAPE_KEY_UP] = false; eventQ.push({ ESCAPE_KEY_DOWN }); break;
        case (SDLK_EQUALS): keyStates[EQUALS_KEY_DOWN] = true; keyStates[EQUALS_KEY_UP] = false; eventQ.push({ EQUALS_KEY_DOWN }); break;
        case (SDLK_0): keyStates[ZERO_KEY_DOWN] = true; keyStates[ZERO_KEY_UP] = false; eventQ.push({ ZERO_KEY_DOWN }); break;
        case (SDLK_1): keyStates[ONE_KEY_DOWN] = true; keyStates[ONE_KEY_UP] = false; eventQ.push({ ONE_KEY_DOWN }); break;
        case (SDLK_2): keyStates[TWO_KEY_DOWN] = true; keyStates[TWO_KEY_UP] = false; eventQ.push({ TWO_KEY_DOWN }); break;
        case (SDLK_3): keyStates[THREE_KEY_DOWN] = true; keyStates[THREE_KEY_UP] = false; eventQ.push({ THREE_KEY_DOWN }); break;
        case (SDLK_4): keyStates[FOUR_KEY_DOWN] = true; keyStates[FOUR_KEY_UP] = false; eventQ.push({ FOUR_KEY_DOWN }); break;
        case (SDLK_5): keyStates[FIVE_KEY_DOWN] = true; keyStates[FIVE_KEY_UP] = false; eventQ.push({ FIVE_KEY_DOWN }); break;
        case (SDLK_6): keyStates[SIX_KEY_DOWN] = true; keyStates[SIX_KEY_UP] = false; eventQ.push({ SIX_KEY_DOWN }); break;
        case (SDLK_7): keyStates[SEVEN_KEY_DOWN] = true; keyStates[SEVEN_KEY_UP] = false; eventQ.push({ SEVEN_KEY_DOWN }); break;
        case (SDLK_8): keyStates[EIGHT_KEY_DOWN] = true; keyStates[EIGHT_KEY_UP] = false; eventQ.push({ EIGHT_KEY_DOWN }); break;
        case (SDLK_9): keyStates[NINE_KEY_DOWN] = true; keyStates[NINE_KEY_UP] = false; eventQ.push({ NINE_KEY_DOWN }); break;
        case (SDLK_q): keyStates[Q_KEY_DOWN] = true; keyStates[Q_KEY_UP] = false; eventQ.push({ Q_KEY_DOWN }); break;
        case (SDLK_w): keyStates[W_KEY_DOWN] = true; keyStates[W_KEY_UP] = false; eventQ.push({ W_KEY_DOWN }); break;
        case (SDLK_e): keyStates[E_KEY_DOWN] = true; keyStates[E_KEY_UP] = false; eventQ.push({ E_KEY_DOWN }); break;
        case (SDLK_r): keyStates[R_KEY_DOWN] = true; keyStates[R_KEY_UP] = false; eventQ.push({ R_KEY_DOWN }); break;
        case (SDLK_t): keyStates[T_KEY_DOWN] = true; keyStates[T_KEY_UP] = false; eventQ.push({ T_KEY_DOWN }); break;
        case (SDLK_y): keyStates[Y_KEY_DOWN] = true; keyStates[Y_KEY_UP] = false; eventQ.push({ Y_KEY_DOWN }); break;
        case (SDLK_u): keyStates[U_KEY_DOWN] = true; keyStates[U_KEY_UP] = false; eventQ.push({ U_KEY_DOWN }); break;
        case (SDLK_i): keyStates[I_KEY_DOWN] = true; keyStates[I_KEY_UP] = false; eventQ.push({ I_KEY_DOWN }); break;
        case (SDLK_o): keyStates[O_KEY_DOWN] = true; keyStates[O_KEY_UP] = false; eventQ.push({ O_KEY_DOWN }); break;
        case (SDLK_p): keyStates[P_KEY_DOWN] = true; keyStates[P_KEY_UP] = false; eventQ.push({ P_KEY_DOWN }); break;
        case (SDLK_a): keyStates[A_KEY_DOWN] = true; keyStates[A_KEY_UP] = false; eventQ.push({ A_KEY_DOWN }); break;
        case (SDLK_s): keyStates[S_KEY_DOWN] = true; keyStates[S_KEY_UP] = false; eventQ.push({ S_KEY_DOWN }); break;
        case (SDLK_d): keyStates[D_KEY_DOWN] = true; keyStates[D_KEY_UP] = false; eventQ.push({ D_KEY_DOWN }); break;
        case (SDLK_f): keyStates[F_KEY_DOWN] = true; keyStates[F_KEY_UP] = false; eventQ.push({ F_KEY_DOWN }); break;
        case (SDLK_g): keyStates[G_KEY_DOWN] = true; keyStates[G_KEY_UP] = false; eventQ.push({ G_KEY_DOWN }); break;
        case (SDLK_h): keyStates[H_KEY_DOWN] = true; keyStates[H_KEY_UP] = false; eventQ.push({ H_KEY_DOWN }); break;
        case (SDLK_j): keyStates[J_KEY_DOWN] = true; keyStates[J_KEY_UP] = false; eventQ.push({ J_KEY_DOWN }); break;
        case (SDLK_k): keyStates[K_KEY_DOWN] = true; keyStates[K_KEY_UP] = false; eventQ.push({ K_KEY_DOWN }); break;
        case (SDLK_l): keyStates[L_KEY_DOWN] = true; keyStates[L_KEY_UP] = false; eventQ.push({ L_KEY_DOWN }); break;
        case (SDLK_z): keyStates[Z_KEY_DOWN] = true; keyStates[Z_KEY_UP] = false; eventQ.push({ Z_KEY_DOWN }); break;
        case (SDLK_x): keyStates[X_KEY_DOWN] = true; keyStates[X_KEY_UP] = false; eventQ.push({ X_KEY_DOWN }); break;
        case (SDLK_c): keyStates[C_KEY_DOWN] = true; keyStates[C_KEY_UP] = false; eventQ.push({ C_KEY_DOWN }); break;
        case (SDLK_v): keyStates[V_KEY_DOWN] = true; keyStates[V_KEY_UP] = false; eventQ.push({ V_KEY_DOWN }); break;
        case (SDLK_b): keyStates[B_KEY_DOWN] = true; keyStates[B_KEY_UP] = false; eventQ.push({ B_KEY_DOWN }); break;
        case (SDLK_n): keyStates[N_KEY_DOWN] = true; keyStates[N_KEY_UP] = false; eventQ.push({ N_KEY_DOWN }); break;
        case (SDLK_m): keyStates[M_KEY_DOWN] = true; keyStates[M_KEY_UP] = false; eventQ.push({ M_KEY_DOWN }); break;
        case (SDLK_TAB): keyStates[TAB_KEY_DOWN] = true; keyStates[TAB_KEY_UP] = false; eventQ.push({ TAB_KEY_DOWN }); break;
        case (SDLK_CAPSLOCK): keyStates[CAPS_LOCK_KEY_DOWN] = true; keyStates[CAPS_LOCK_KEY_UP] = false; eventQ.push({ CAPS_LOCK_KEY_DOWN }); break;
        case (SDLK_LSHIFT): keyStates[LEFT_SHIFT_KEY_DOWN] = true; keyStates[LEFT_SHIFT_KEY_UP] = false; eventQ.push({ LEFT_SHIFT_KEY_DOWN }); break;
        case (SDLK_RSHIFT): keyStates[RIGHT_SHIFT_KEY_DOWN] = true; keyStates[RIGHT_SHIFT_KEY_UP] = false; eventQ.push({ RIGHT_SHIFT_KEY_DOWN }); break;
        case (SDLK_RETURN): keyStates[ENTER_KEY_DOWN] = true; keyStates[ENTER_KEY_UP] = false; eventQ.push({ ENTER_KEY_DOWN }); break;
        case (SDLK_LEFT): keyStates[LEFT_ARROW_KEY_DOWN] = true; keyStates[LEFT_ARROW_KEY_UP] = false; eventQ.push({ LEFT_ARROW_KEY_DOWN }); break;
        case (SDLK_UP): keyStates[UP_ARROW_KEY_DOWN] = true; keyStates[UP_ARROW_KEY_UP] = false; eventQ.push({ UP_ARROW_KEY_DOWN }); break;
        case (SDLK_RIGHT): keyStates[RIGHT_ARROW_KEY_DOWN] = true; keyStates[RIGHT_ARROW_KEY_UP] = false; eventQ.push({ RIGHT_ARROW_KEY_DOWN }); break;
        case (SDLK_DOWN): keyStates[DOWN_ARROW_KEY_DOWN] = true; keyStates[DOWN_ARROW_KEY_UP] = false; eventQ.push({ DOWN_ARROW_KEY_DOWN }); break;
        case (SDLK_SPACE): keyStates[SPACE_KEY_DOWN] = true; keyStates[SPACE_KEY_UP] = false; eventQ.push({ SPACE_KEY_DOWN }); break;
        case (SDLK_MINUS): keyStates[MINUS_KEY_DOWN] = true; keyStates[MINUS_KEY_UP] = false; eventQ.push({ MINUS_KEY_DOWN }); break;
        case (SDLK_BACKSPACE): keyStates[BACKSPACE_KEY_DOWN] = true; keyStates[BACKSPACE_KEY_UP] = false; eventQ.push({ BACKSPACE_KEY_DOWN }); break;
        default: break;
        }
    }

    void EventManager::dispatchKeyupEvent(SDL_Keycode& keyCode)
    {
        switch (keyCode)
        {
        case (SDLK_ESCAPE): keyStates[ESCAPE_KEY_UP] = true; keyStates[ESCAPE_KEY_DOWN] = false; eventQ.push({ ESCAPE_KEY_UP }); break;
        case (SDLK_EQUALS): keyStates[EQUALS_KEY_UP] = true; keyStates[EQUALS_KEY_DOWN] = false; eventQ.push({ EQUALS_KEY_UP }); break;
        case (SDLK_0): keyStates[ZERO_KEY_UP] = true; keyStates[ZERO_KEY_DOWN] = false; eventQ.push({ ZERO_KEY_UP }); break;
        case (SDLK_1): keyStates[ONE_KEY_UP] = true; keyStates[ONE_KEY_DOWN] = false; eventQ.push({ ONE_KEY_UP }); break;
        case (SDLK_2): keyStates[TWO_KEY_UP] = true; keyStates[TWO_KEY_DOWN] = false; eventQ.push({ TWO_KEY_UP }); break;
        case (SDLK_3): keyStates[THREE_KEY_UP] = true; keyStates[THREE_KEY_DOWN] = false; eventQ.push({ THREE_KEY_UP }); break;
        case (SDLK_4): keyStates[FOUR_KEY_UP] = true; keyStates[FOUR_KEY_DOWN] = false; eventQ.push({ FOUR_KEY_UP }); break;
        case (SDLK_5): keyStates[FIVE_KEY_UP] = true; keyStates[FIVE_KEY_DOWN] = false; eventQ.push({ FIVE_KEY_UP }); break;
        case (SDLK_6): keyStates[SIX_KEY_UP] = true; keyStates[SIX_KEY_DOWN] = false; eventQ.push({ SIX_KEY_UP }); break;
        case (SDLK_7): keyStates[SEVEN_KEY_UP] = true; keyStates[SEVEN_KEY_DOWN] = false; eventQ.push({ SEVEN_KEY_UP }); break;
        case (SDLK_8): keyStates[EIGHT_KEY_UP] = true; keyStates[EIGHT_KEY_DOWN] = false; eventQ.push({ EIGHT_KEY_UP }); break;
        case (SDLK_9): keyStates[NINE_KEY_UP] = true; keyStates[NINE_KEY_DOWN] = false; eventQ.push({ NINE_KEY_UP }); break;
        case (SDLK_q): keyStates[Q_KEY_UP] = true; keyStates[Q_KEY_DOWN] = false; eventQ.push({ Q_KEY_UP }); break;
        case (SDLK_w): keyStates[W_KEY_UP] = true; keyStates[W_KEY_DOWN] = false; eventQ.push({ W_KEY_UP }); break;
        case (SDLK_e): keyStates[E_KEY_UP] = true; keyStates[E_KEY_DOWN] = false; eventQ.push({ E_KEY_UP }); break;
        case (SDLK_r): keyStates[R_KEY_UP] = true; keyStates[R_KEY_DOWN] = false; eventQ.push({ R_KEY_UP }); break;
        case (SDLK_t): keyStates[T_KEY_UP] = true; keyStates[T_KEY_DOWN] = false; eventQ.push({ T_KEY_UP }); break;
        case (SDLK_y): keyStates[Y_KEY_UP] = true; keyStates[Y_KEY_DOWN] = false; eventQ.push({ Y_KEY_UP }); break;
        case (SDLK_u): keyStates[U_KEY_UP] = true; keyStates[U_KEY_DOWN] = false; eventQ.push({ U_KEY_UP }); break;
        case (SDLK_i): keyStates[I_KEY_UP] = true; keyStates[I_KEY_DOWN] = false; eventQ.push({ I_KEY_UP }); break;
        case (SDLK_o): keyStates[O_KEY_UP] = true; keyStates[O_KEY_DOWN] = false; eventQ.push({ O_KEY_UP }); break;
        case (SDLK_p): keyStates[P_KEY_UP] = true; keyStates[P_KEY_DOWN] = false; eventQ.push({ P_KEY_UP }); break;
        case (SDLK_a): keyStates[A_KEY_UP] = true; keyStates[A_KEY_DOWN] = false; eventQ.push({ A_KEY_UP }); break;
        case (SDLK_s): keyStates[S_KEY_UP] = true; keyStates[S_KEY_DOWN] = false; eventQ.push({ S_KEY_UP }); break;
        case (SDLK_d): keyStates[D_KEY_UP] = true; keyStates[D_KEY_DOWN] = false; eventQ.push({ D_KEY_UP }); break;
        case (SDLK_f): keyStates[F_KEY_UP] = true; keyStates[F_KEY_DOWN] = false; eventQ.push({ F_KEY_UP }); break;
        case (SDLK_g): keyStates[G_KEY_UP] = true; keyStates[G_KEY_DOWN] = false; eventQ.push({ G_KEY_UP }); break;
        case (SDLK_h): keyStates[H_KEY_UP] = true; keyStates[H_KEY_DOWN] = false; eventQ.push({ H_KEY_UP }); break;
        case (SDLK_j): keyStates[J_KEY_UP] = true; keyStates[J_KEY_DOWN] = false; eventQ.push({ J_KEY_UP }); break;
        case (SDLK_k): keyStates[K_KEY_UP] = true; keyStates[K_KEY_DOWN] = false; eventQ.push({ K_KEY_UP }); break;
        case (SDLK_l): keyStates[L_KEY_UP] = true; keyStates[L_KEY_DOWN] = false; eventQ.push({ L_KEY_UP }); break;
        case (SDLK_z): keyStates[Z_KEY_UP] = true; keyStates[Z_KEY_DOWN] = false; eventQ.push({ Z_KEY_UP }); break;
        case (SDLK_x): keyStates[X_KEY_UP] = true; keyStates[X_KEY_DOWN] = false; eventQ.push({ X_KEY_UP }); break;
        case (SDLK_c): keyStates[C_KEY_UP] = true; keyStates[C_KEY_DOWN] = false; eventQ.push({ C_KEY_UP }); break;
        case (SDLK_v): keyStates[V_KEY_UP] = true; keyStates[V_KEY_DOWN] = false; eventQ.push({ V_KEY_UP }); break;
        case (SDLK_b): keyStates[B_KEY_UP] = true; keyStates[B_KEY_DOWN] = false; eventQ.push({ B_KEY_UP }); break;
        case (SDLK_n): keyStates[N_KEY_UP] = true; keyStates[N_KEY_DOWN] = false; eventQ.push({ N_KEY_UP }); break;
        case (SDLK_m): keyStates[M_KEY_UP] = true; keyStates[M_KEY_DOWN] = false; eventQ.push({ M_KEY_UP }); break;
        case (SDLK_TAB): keyStates[TAB_KEY_UP] = true; keyStates[TAB_KEY_DOWN] = false; eventQ.push({ TAB_KEY_UP }); break;
        case (SDLK_CAPSLOCK): keyStates[CAPS_LOCK_KEY_UP] = true; keyStates[CAPS_LOCK_KEY_DOWN] = false; eventQ.push({ CAPS_LOCK_KEY_UP }); break;
        case (SDLK_LSHIFT): keyStates[LEFT_SHIFT_KEY_UP] = true; keyStates[LEFT_SHIFT_KEY_DOWN] = false; eventQ.push({ LEFT_SHIFT_KEY_UP }); break;
        case (SDLK_RSHIFT): keyStates[RIGHT_SHIFT_KEY_UP] = true; keyStates[RIGHT_SHIFT_KEY_DOWN] = false; eventQ.push({ RIGHT_SHIFT_KEY_UP }); break;
        case (SDLK_RETURN): keyStates[ENTER_KEY_UP] = true; keyStates[ENTER_KEY_DOWN] = false; eventQ.push({ ENTER_KEY_UP }); break;
        case (SDLK_LEFT): keyStates[LEFT_ARROW_KEY_UP] = true; keyStates[LEFT_ARROW_KEY_DOWN] = false; eventQ.push({ LEFT_ARROW_KEY_UP }); break;
        case (SDLK_UP): keyStates[UP_ARROW_KEY_UP] = true; keyStates[UP_ARROW_KEY_DOWN] = false; eventQ.push({ UP_ARROW_KEY_UP }); break;
        case (SDLK_RIGHT): keyStates[RIGHT_ARROW_KEY_UP] = true; keyStates[RIGHT_ARROW_KEY_DOWN] = false; eventQ.push({ RIGHT_ARROW_KEY_UP }); break;
        case (SDLK_DOWN): keyStates[DOWN_ARROW_KEY_UP] = true; keyStates[DOWN_ARROW_KEY_DOWN] = false; eventQ.push({ DOWN_ARROW_KEY_UP }); break;
        case (SDLK_SPACE): keyStates[SPACE_KEY_UP] = true; keyStates[SPACE_KEY_DOWN] = false; eventQ.push({ SPACE_KEY_UP }); break;
        case (SDLK_MINUS): keyStates[MINUS_KEY_UP] = true; keyStates[MINUS_KEY_DOWN] = false; eventQ.push({ MINUS_KEY_UP }); break;
        case (SDLK_BACKSPACE): keyStates[BACKSPACE_KEY_UP] = true; keyStates[BACKSPACE_KEY_DOWN] = false; eventQ.push({ BACKSPACE_KEY_UP }); break;
        default: break;
        }
    }
    void EventManager::dispatchMouseMoveEvent(SDL_MouseMotionEvent& mouseMotion)
    {
        eventQ.push({ MOUSE_MOVE, mouseMotion.x, mouseMotion.y });
    }
    void EventManager::dispatchMouseButtonDownEvent(SDL_MouseButtonEvent& mouseButtonEvent)
    {
        switch (mouseButtonEvent.button)
        {
        case (SDL_BUTTON_LEFT): eventQ.push({ LEFT_MOUSE_BUTTON_DOWN }); break;
        case (SDL_BUTTON_MIDDLE): eventQ.push({ MIDDLE_MOUSE_BUTTON_DOWN }); break;
        case (SDL_BUTTON_RIGHT): eventQ.push({ RIGHT_MOUSE_BUTTON_DOWN }); break;
        default: break;
        }
    }
    void EventManager::dispatchMouseButtonUpEvent(SDL_MouseButtonEvent& mouseButtonEvent)
    {
        switch (mouseButtonEvent.button)
        {
        case (SDL_BUTTON_LEFT): eventQ.push({ LEFT_MOUSE_BUTTON_UP }); break;
        case (SDL_BUTTON_MIDDLE): eventQ.push({ MIDDLE_MOUSE_BUTTON_UP }); break;
        case (SDL_BUTTON_RIGHT): eventQ.push({ RIGHT_MOUSE_BUTTON_UP }); break;
        default: break;
        }
    }
    void EventManager::dispatchMouseScrollEvent(SDL_MouseWheelEvent& mouseWheelEvent)
    {
        switch (mouseWheelEvent.direction)
        {
        case (SDL_MOUSEWHEEL_NORMAL): eventQ.push({ MOUSE_WHEEL_MOVED, mouseWheelEvent.x, mouseWheelEvent.y }); break;
        default: break;
        }
    }
}