#pragma once

#include <queue>

#include "../../include/SDLWrapper/SDLWrapper.h"
#include "../../include/Events/EventManager.h"
#include "../../include/Events/Event.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
    EventManager* EventManager::instance = nullptr;

    EventManager::EventManager() {
        // Initialize all key down states to false
        m_keyStates[ESCAPE_KEY_DOWN] = false;
        m_keyStates[EQUALS_KEY_DOWN] = false;
        m_keyStates[ZERO_KEY_DOWN] = false;
        m_keyStates[ONE_KEY_DOWN] = false;
        m_keyStates[TWO_KEY_DOWN] = false;
        m_keyStates[THREE_KEY_DOWN] = false;
        m_keyStates[FOUR_KEY_DOWN] = false;
        m_keyStates[FIVE_KEY_DOWN] = false;
        m_keyStates[SIX_KEY_DOWN] = false;
        m_keyStates[SEVEN_KEY_DOWN] = false;
        m_keyStates[EIGHT_KEY_DOWN] = false;
        m_keyStates[NINE_KEY_DOWN] = false;
        m_keyStates[Q_KEY_DOWN] = false;
        m_keyStates[W_KEY_DOWN] = false;
        m_keyStates[E_KEY_DOWN] = false;
        m_keyStates[R_KEY_DOWN] = false;
        m_keyStates[T_KEY_DOWN] = false;
        m_keyStates[Y_KEY_DOWN] = false;
        m_keyStates[U_KEY_DOWN] = false;
        m_keyStates[I_KEY_DOWN] = false;
        m_keyStates[O_KEY_DOWN] = false;
        m_keyStates[P_KEY_DOWN] = false;
        m_keyStates[A_KEY_DOWN] = false;
        m_keyStates[S_KEY_DOWN] = false;
        m_keyStates[D_KEY_DOWN] = false;
        m_keyStates[F_KEY_DOWN] = false;
        m_keyStates[G_KEY_DOWN] = false;
        m_keyStates[H_KEY_DOWN] = false;
        m_keyStates[J_KEY_DOWN] = false;
        m_keyStates[K_KEY_DOWN] = false;
        m_keyStates[L_KEY_DOWN] = false;
        m_keyStates[Z_KEY_DOWN] = false;
        m_keyStates[X_KEY_DOWN] = false;
        m_keyStates[C_KEY_DOWN] = false;
        m_keyStates[V_KEY_DOWN] = false;
        m_keyStates[B_KEY_DOWN] = false;
        m_keyStates[N_KEY_DOWN] = false;
        m_keyStates[M_KEY_DOWN] = false;
        m_keyStates[TAB_KEY_DOWN] = false;
        m_keyStates[CAPS_LOCK_KEY_DOWN] = false;
        m_keyStates[LEFT_SHIFT_KEY_DOWN] = false;
        m_keyStates[RIGHT_SHIFT_KEY_DOWN] = false;
        m_keyStates[ENTER_KEY_DOWN] = false;
        m_keyStates[LEFT_ARROW_KEY_DOWN] = false;
        m_keyStates[UP_ARROW_KEY_DOWN] = false;
        m_keyStates[RIGHT_ARROW_KEY_DOWN] = false;
        m_keyStates[DOWN_ARROW_KEY_DOWN] = false;
        m_keyStates[SPACE_KEY_DOWN] = false;
        m_keyStates[MINUS_KEY_DOWN] = false;
        m_keyStates[BACKSPACE_KEY_DOWN] = false;

        // Initialize all key up states to true
        m_keyStates[ESCAPE_KEY_UP] = true;
        m_keyStates[EQUALS_KEY_UP] = true;
        m_keyStates[ZERO_KEY_UP] = true;
        m_keyStates[ONE_KEY_UP] = true;
        m_keyStates[TWO_KEY_UP] = true;
        m_keyStates[THREE_KEY_UP] = true;
        m_keyStates[FOUR_KEY_UP] = true;
        m_keyStates[FIVE_KEY_UP] = true;
        m_keyStates[SIX_KEY_UP] = true;
        m_keyStates[SEVEN_KEY_UP] = true;
        m_keyStates[EIGHT_KEY_UP] = true;
        m_keyStates[NINE_KEY_UP] = true;
        m_keyStates[Q_KEY_UP] = true;
        m_keyStates[W_KEY_UP] = true;
        m_keyStates[E_KEY_UP] = true;
        m_keyStates[R_KEY_UP] = true;
        m_keyStates[T_KEY_UP] = true;
        m_keyStates[Y_KEY_UP] = true;
        m_keyStates[U_KEY_UP] = true;
        m_keyStates[I_KEY_UP] = true;
        m_keyStates[O_KEY_UP] = true;
        m_keyStates[P_KEY_UP] = true;
        m_keyStates[A_KEY_UP] = true;
        m_keyStates[S_KEY_UP] = true;
        m_keyStates[D_KEY_UP] = true;
        m_keyStates[F_KEY_UP] = true;
        m_keyStates[G_KEY_UP] = true;
        m_keyStates[H_KEY_UP] = true;
        m_keyStates[J_KEY_UP] = true;
        m_keyStates[K_KEY_UP] = true;
        m_keyStates[L_KEY_UP] = true;
        m_keyStates[Z_KEY_UP] = true;
        m_keyStates[X_KEY_UP] = true;
        m_keyStates[C_KEY_UP] = true;
        m_keyStates[V_KEY_UP] = true;
        m_keyStates[B_KEY_UP] = true;
        m_keyStates[N_KEY_UP] = true;
        m_keyStates[M_KEY_UP] = true;
        m_keyStates[TAB_KEY_UP] = true;
        m_keyStates[CAPS_LOCK_KEY_UP] = true;
        m_keyStates[LEFT_SHIFT_KEY_UP] = true;
        m_keyStates[RIGHT_SHIFT_KEY_UP] = true;
        m_keyStates[ENTER_KEY_UP] = true;
        m_keyStates[LEFT_ARROW_KEY_UP] = true;
        m_keyStates[UP_ARROW_KEY_UP] = true;
        m_keyStates[RIGHT_ARROW_KEY_UP] = true;
        m_keyStates[DOWN_ARROW_KEY_UP] = true;
        m_keyStates[SPACE_KEY_UP] = true;
        m_keyStates[MINUS_KEY_UP] = true;
        m_keyStates[BACKSPACE_KEY_UP] = true;
    }

	EventManager* EventManager::GetInstance()
	{
        if (instance == nullptr)
        {
			instance = new EventManager();
		}
		return instance;
	}

    /*const std::unordered_map<EventType, bool>& EventManager::GetKeyStates() const { return keyStates; }
    const std::pair<int, int>& EventManager::GetMousePosition() const { return mousePosition; }
    const std::pair<int, int>& EventManager::GetScrollDirection() const { return scrollDirection; }
	const std::unordered_map<EventType, bool>& EventManager::GetMouseButtonStates() const { return mouseButtonStates; }*/

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
        case (SDLK_ESCAPE): m_keyStates[ESCAPE_KEY_DOWN] = true; m_keyStates[ESCAPE_KEY_UP] = false; eventQ.push({ ESCAPE_KEY_DOWN }); break;
        case (SDLK_EQUALS): m_keyStates[EQUALS_KEY_DOWN] = true; m_keyStates[EQUALS_KEY_UP] = false; eventQ.push({ EQUALS_KEY_DOWN }); break;
        case (SDLK_0): m_keyStates[ZERO_KEY_DOWN] = true; m_keyStates[ZERO_KEY_UP] = false; eventQ.push({ ZERO_KEY_DOWN }); break;
        case (SDLK_1): m_keyStates[ONE_KEY_DOWN] = true; m_keyStates[ONE_KEY_UP] = false; eventQ.push({ ONE_KEY_DOWN }); break;
        case (SDLK_2): m_keyStates[TWO_KEY_DOWN] = true; m_keyStates[TWO_KEY_UP] = false; eventQ.push({ TWO_KEY_DOWN }); break;
        case (SDLK_3): m_keyStates[THREE_KEY_DOWN] = true; m_keyStates[THREE_KEY_UP] = false; eventQ.push({ THREE_KEY_DOWN }); break;
        case (SDLK_4): m_keyStates[FOUR_KEY_DOWN] = true; m_keyStates[FOUR_KEY_UP] = false; eventQ.push({ FOUR_KEY_DOWN }); break;
        case (SDLK_5): m_keyStates[FIVE_KEY_DOWN] = true; m_keyStates[FIVE_KEY_UP] = false; eventQ.push({ FIVE_KEY_DOWN }); break;
        case (SDLK_6): m_keyStates[SIX_KEY_DOWN] = true; m_keyStates[SIX_KEY_UP] = false; eventQ.push({ SIX_KEY_DOWN }); break;
        case (SDLK_7): m_keyStates[SEVEN_KEY_DOWN] = true; m_keyStates[SEVEN_KEY_UP] = false; eventQ.push({ SEVEN_KEY_DOWN }); break;
        case (SDLK_8): m_keyStates[EIGHT_KEY_DOWN] = true; m_keyStates[EIGHT_KEY_UP] = false; eventQ.push({ EIGHT_KEY_DOWN }); break;
        case (SDLK_9): m_keyStates[NINE_KEY_DOWN] = true; m_keyStates[NINE_KEY_UP] = false; eventQ.push({ NINE_KEY_DOWN }); break;
        case (SDLK_q): m_keyStates[Q_KEY_DOWN] = true; m_keyStates[Q_KEY_UP] = false; eventQ.push({ Q_KEY_DOWN }); break;
        case (SDLK_w): m_keyStates[W_KEY_DOWN] = true; m_keyStates[W_KEY_UP] = false; eventQ.push({ W_KEY_DOWN }); break;
        case (SDLK_e): m_keyStates[E_KEY_DOWN] = true; m_keyStates[E_KEY_UP] = false; eventQ.push({ E_KEY_DOWN }); break;
        case (SDLK_r): m_keyStates[R_KEY_DOWN] = true; m_keyStates[R_KEY_UP] = false; eventQ.push({ R_KEY_DOWN }); break;
        case (SDLK_t): m_keyStates[T_KEY_DOWN] = true; m_keyStates[T_KEY_UP] = false; eventQ.push({ T_KEY_DOWN }); break;
        case (SDLK_y): m_keyStates[Y_KEY_DOWN] = true; m_keyStates[Y_KEY_UP] = false; eventQ.push({ Y_KEY_DOWN }); break;
        case (SDLK_u): m_keyStates[U_KEY_DOWN] = true; m_keyStates[U_KEY_UP] = false; eventQ.push({ U_KEY_DOWN }); break;
        case (SDLK_i): m_keyStates[I_KEY_DOWN] = true; m_keyStates[I_KEY_UP] = false; eventQ.push({ I_KEY_DOWN }); break;
        case (SDLK_o): m_keyStates[O_KEY_DOWN] = true; m_keyStates[O_KEY_UP] = false; eventQ.push({ O_KEY_DOWN }); break;
        case (SDLK_p): m_keyStates[P_KEY_DOWN] = true; m_keyStates[P_KEY_UP] = false; eventQ.push({ P_KEY_DOWN }); break;
        case (SDLK_a): m_keyStates[A_KEY_DOWN] = true; m_keyStates[A_KEY_UP] = false; eventQ.push({ A_KEY_DOWN }); break;
        case (SDLK_s): m_keyStates[S_KEY_DOWN] = true; m_keyStates[S_KEY_UP] = false; eventQ.push({ S_KEY_DOWN }); break;
        case (SDLK_d): m_keyStates[D_KEY_DOWN] = true; m_keyStates[D_KEY_UP] = false; eventQ.push({ D_KEY_DOWN }); break;
        case (SDLK_f): m_keyStates[F_KEY_DOWN] = true; m_keyStates[F_KEY_UP] = false; eventQ.push({ F_KEY_DOWN }); break;
        case (SDLK_g): m_keyStates[G_KEY_DOWN] = true; m_keyStates[G_KEY_UP] = false; eventQ.push({ G_KEY_DOWN }); break;
        case (SDLK_h): m_keyStates[H_KEY_DOWN] = true; m_keyStates[H_KEY_UP] = false; eventQ.push({ H_KEY_DOWN }); break;
        case (SDLK_j): m_keyStates[J_KEY_DOWN] = true; m_keyStates[J_KEY_UP] = false; eventQ.push({ J_KEY_DOWN }); break;
        case (SDLK_k): m_keyStates[K_KEY_DOWN] = true; m_keyStates[K_KEY_UP] = false; eventQ.push({ K_KEY_DOWN }); break;
        case (SDLK_l): m_keyStates[L_KEY_DOWN] = true; m_keyStates[L_KEY_UP] = false; eventQ.push({ L_KEY_DOWN }); break;
        case (SDLK_z): m_keyStates[Z_KEY_DOWN] = true; m_keyStates[Z_KEY_UP] = false; eventQ.push({ Z_KEY_DOWN }); break;
        case (SDLK_x): m_keyStates[X_KEY_DOWN] = true; m_keyStates[X_KEY_UP] = false; eventQ.push({ X_KEY_DOWN }); break;
        case (SDLK_c): m_keyStates[C_KEY_DOWN] = true; m_keyStates[C_KEY_UP] = false; eventQ.push({ C_KEY_DOWN }); break;
        case (SDLK_v): m_keyStates[V_KEY_DOWN] = true; m_keyStates[V_KEY_UP] = false; eventQ.push({ V_KEY_DOWN }); break;
        case (SDLK_b): m_keyStates[B_KEY_DOWN] = true; m_keyStates[B_KEY_UP] = false; eventQ.push({ B_KEY_DOWN }); break;
        case (SDLK_n): m_keyStates[N_KEY_DOWN] = true; m_keyStates[N_KEY_UP] = false; eventQ.push({ N_KEY_DOWN }); break;
        case (SDLK_m): m_keyStates[M_KEY_DOWN] = true; m_keyStates[M_KEY_UP] = false; eventQ.push({ M_KEY_DOWN }); break;
        case (SDLK_TAB): m_keyStates[TAB_KEY_DOWN] = true; m_keyStates[TAB_KEY_UP] = false; eventQ.push({ TAB_KEY_DOWN }); break;
        case (SDLK_CAPSLOCK): m_keyStates[CAPS_LOCK_KEY_DOWN] = true; m_keyStates[CAPS_LOCK_KEY_UP] = false; eventQ.push({ CAPS_LOCK_KEY_DOWN }); break;
        case (SDLK_LSHIFT): m_keyStates[LEFT_SHIFT_KEY_DOWN] = true; m_keyStates[LEFT_SHIFT_KEY_UP] = false; eventQ.push({ LEFT_SHIFT_KEY_DOWN }); break;
        case (SDLK_RSHIFT): m_keyStates[RIGHT_SHIFT_KEY_DOWN] = true; m_keyStates[RIGHT_SHIFT_KEY_UP] = false; eventQ.push({ RIGHT_SHIFT_KEY_DOWN }); break;
        case (SDLK_RETURN): m_keyStates[ENTER_KEY_DOWN] = true; m_keyStates[ENTER_KEY_UP] = false; eventQ.push({ ENTER_KEY_DOWN }); break;
        case (SDLK_LEFT): m_keyStates[LEFT_ARROW_KEY_DOWN] = true; m_keyStates[LEFT_ARROW_KEY_UP] = false; eventQ.push({ LEFT_ARROW_KEY_DOWN }); break;
        case (SDLK_UP): m_keyStates[UP_ARROW_KEY_DOWN] = true; m_keyStates[UP_ARROW_KEY_UP] = false; eventQ.push({ UP_ARROW_KEY_DOWN }); break;
        case (SDLK_RIGHT): m_keyStates[RIGHT_ARROW_KEY_DOWN] = true; m_keyStates[RIGHT_ARROW_KEY_UP] = false; eventQ.push({ RIGHT_ARROW_KEY_DOWN }); break;
        case (SDLK_DOWN): m_keyStates[DOWN_ARROW_KEY_DOWN] = true; m_keyStates[DOWN_ARROW_KEY_UP] = false; eventQ.push({ DOWN_ARROW_KEY_DOWN }); break;
        case (SDLK_SPACE): m_keyStates[SPACE_KEY_DOWN] = true; m_keyStates[SPACE_KEY_UP] = false; eventQ.push({ SPACE_KEY_DOWN }); break;
        case (SDLK_MINUS): m_keyStates[MINUS_KEY_DOWN] = true; m_keyStates[MINUS_KEY_UP] = false; eventQ.push({ MINUS_KEY_DOWN }); break;
        case (SDLK_BACKSPACE): m_keyStates[BACKSPACE_KEY_DOWN] = true; m_keyStates[BACKSPACE_KEY_UP] = false; eventQ.push({ BACKSPACE_KEY_DOWN }); break;
        default: break;
        }
    }

    void EventManager::dispatchKeyupEvent(SDL_Keycode& keyCode)
    {
        switch (keyCode)
        {
        case (SDLK_ESCAPE): m_keyStates[ESCAPE_KEY_UP] = true; m_keyStates[ESCAPE_KEY_DOWN] = false; eventQ.push({ ESCAPE_KEY_UP }); break;
        case (SDLK_EQUALS): m_keyStates[EQUALS_KEY_UP] = true; m_keyStates[EQUALS_KEY_DOWN] = false; eventQ.push({ EQUALS_KEY_UP }); break;
        case (SDLK_0): m_keyStates[ZERO_KEY_UP] = true; m_keyStates[ZERO_KEY_DOWN] = false; eventQ.push({ ZERO_KEY_UP }); break;
        case (SDLK_1): m_keyStates[ONE_KEY_UP] = true; m_keyStates[ONE_KEY_DOWN] = false; eventQ.push({ ONE_KEY_UP }); break;
        case (SDLK_2): m_keyStates[TWO_KEY_UP] = true; m_keyStates[TWO_KEY_DOWN] = false; eventQ.push({ TWO_KEY_UP }); break;
        case (SDLK_3): m_keyStates[THREE_KEY_UP] = true; m_keyStates[THREE_KEY_DOWN] = false; eventQ.push({ THREE_KEY_UP }); break;
        case (SDLK_4): m_keyStates[FOUR_KEY_UP] = true; m_keyStates[FOUR_KEY_DOWN] = false; eventQ.push({ FOUR_KEY_UP }); break;
        case (SDLK_5): m_keyStates[FIVE_KEY_UP] = true; m_keyStates[FIVE_KEY_DOWN] = false; eventQ.push({ FIVE_KEY_UP }); break;
        case (SDLK_6): m_keyStates[SIX_KEY_UP] = true; m_keyStates[SIX_KEY_DOWN] = false; eventQ.push({ SIX_KEY_UP }); break;
        case (SDLK_7): m_keyStates[SEVEN_KEY_UP] = true; m_keyStates[SEVEN_KEY_DOWN] = false; eventQ.push({ SEVEN_KEY_UP }); break;
        case (SDLK_8): m_keyStates[EIGHT_KEY_UP] = true; m_keyStates[EIGHT_KEY_DOWN] = false; eventQ.push({ EIGHT_KEY_UP }); break;
        case (SDLK_9): m_keyStates[NINE_KEY_UP] = true; m_keyStates[NINE_KEY_DOWN] = false; eventQ.push({ NINE_KEY_UP }); break;
        case (SDLK_q): m_keyStates[Q_KEY_UP] = true; m_keyStates[Q_KEY_DOWN] = false; eventQ.push({ Q_KEY_UP }); break;
        case (SDLK_w): m_keyStates[W_KEY_UP] = true; m_keyStates[W_KEY_DOWN] = false; eventQ.push({ W_KEY_UP }); break;
        case (SDLK_e): m_keyStates[E_KEY_UP] = true; m_keyStates[E_KEY_DOWN] = false; eventQ.push({ E_KEY_UP }); break;
        case (SDLK_r): m_keyStates[R_KEY_UP] = true; m_keyStates[R_KEY_DOWN] = false; eventQ.push({ R_KEY_UP }); break;
        case (SDLK_t): m_keyStates[T_KEY_UP] = true; m_keyStates[T_KEY_DOWN] = false; eventQ.push({ T_KEY_UP }); break;
        case (SDLK_y): m_keyStates[Y_KEY_UP] = true; m_keyStates[Y_KEY_DOWN] = false; eventQ.push({ Y_KEY_UP }); break;
        case (SDLK_u): m_keyStates[U_KEY_UP] = true; m_keyStates[U_KEY_DOWN] = false; eventQ.push({ U_KEY_UP }); break;
        case (SDLK_i): m_keyStates[I_KEY_UP] = true; m_keyStates[I_KEY_DOWN] = false; eventQ.push({ I_KEY_UP }); break;
        case (SDLK_o): m_keyStates[O_KEY_UP] = true; m_keyStates[O_KEY_DOWN] = false; eventQ.push({ O_KEY_UP }); break;
        case (SDLK_p): m_keyStates[P_KEY_UP] = true; m_keyStates[P_KEY_DOWN] = false; eventQ.push({ P_KEY_UP }); break;
        case (SDLK_a): m_keyStates[A_KEY_UP] = true; m_keyStates[A_KEY_DOWN] = false; eventQ.push({ A_KEY_UP }); break;
        case (SDLK_s): m_keyStates[S_KEY_UP] = true; m_keyStates[S_KEY_DOWN] = false; eventQ.push({ S_KEY_UP }); break;
        case (SDLK_d): m_keyStates[D_KEY_UP] = true; m_keyStates[D_KEY_DOWN] = false; eventQ.push({ D_KEY_UP }); break;
        case (SDLK_f): m_keyStates[F_KEY_UP] = true; m_keyStates[F_KEY_DOWN] = false; eventQ.push({ F_KEY_UP }); break;
        case (SDLK_g): m_keyStates[G_KEY_UP] = true; m_keyStates[G_KEY_DOWN] = false; eventQ.push({ G_KEY_UP }); break;
        case (SDLK_h): m_keyStates[H_KEY_UP] = true; m_keyStates[H_KEY_DOWN] = false; eventQ.push({ H_KEY_UP }); break;
        case (SDLK_j): m_keyStates[J_KEY_UP] = true; m_keyStates[J_KEY_DOWN] = false; eventQ.push({ J_KEY_UP }); break;
        case (SDLK_k): m_keyStates[K_KEY_UP] = true; m_keyStates[K_KEY_DOWN] = false; eventQ.push({ K_KEY_UP }); break;
        case (SDLK_l): m_keyStates[L_KEY_UP] = true; m_keyStates[L_KEY_DOWN] = false; eventQ.push({ L_KEY_UP }); break;
        case (SDLK_z): m_keyStates[Z_KEY_UP] = true; m_keyStates[Z_KEY_DOWN] = false; eventQ.push({ Z_KEY_UP }); break;
        case (SDLK_x): m_keyStates[X_KEY_UP] = true; m_keyStates[X_KEY_DOWN] = false; eventQ.push({ X_KEY_UP }); break;
        case (SDLK_c): m_keyStates[C_KEY_UP] = true; m_keyStates[C_KEY_DOWN] = false; eventQ.push({ C_KEY_UP }); break;
        case (SDLK_v): m_keyStates[V_KEY_UP] = true; m_keyStates[V_KEY_DOWN] = false; eventQ.push({ V_KEY_UP }); break;
        case (SDLK_b): m_keyStates[B_KEY_UP] = true; m_keyStates[B_KEY_DOWN] = false; eventQ.push({ B_KEY_UP }); break;
        case (SDLK_n): m_keyStates[N_KEY_UP] = true; m_keyStates[N_KEY_DOWN] = false; eventQ.push({ N_KEY_UP }); break;
        case (SDLK_m): m_keyStates[M_KEY_UP] = true; m_keyStates[M_KEY_DOWN] = false; eventQ.push({ M_KEY_UP }); break;
        case (SDLK_TAB): m_keyStates[TAB_KEY_UP] = true; m_keyStates[TAB_KEY_DOWN] = false; eventQ.push({ TAB_KEY_UP }); break;
        case (SDLK_CAPSLOCK): m_keyStates[CAPS_LOCK_KEY_UP] = true; m_keyStates[CAPS_LOCK_KEY_DOWN] = false; eventQ.push({ CAPS_LOCK_KEY_UP }); break;
        case (SDLK_LSHIFT): m_keyStates[LEFT_SHIFT_KEY_UP] = true; m_keyStates[LEFT_SHIFT_KEY_DOWN] = false; eventQ.push({ LEFT_SHIFT_KEY_UP }); break;
        case (SDLK_RSHIFT): m_keyStates[RIGHT_SHIFT_KEY_UP] = true; m_keyStates[RIGHT_SHIFT_KEY_DOWN] = false; eventQ.push({ RIGHT_SHIFT_KEY_UP }); break;
        case (SDLK_RETURN): m_keyStates[ENTER_KEY_UP] = true; m_keyStates[ENTER_KEY_DOWN] = false; eventQ.push({ ENTER_KEY_UP }); break;
        case (SDLK_LEFT): m_keyStates[LEFT_ARROW_KEY_UP] = true; m_keyStates[LEFT_ARROW_KEY_DOWN] = false; eventQ.push({ LEFT_ARROW_KEY_UP }); break;
        case (SDLK_UP): m_keyStates[UP_ARROW_KEY_UP] = true; m_keyStates[UP_ARROW_KEY_DOWN] = false; eventQ.push({ UP_ARROW_KEY_UP }); break;
        case (SDLK_RIGHT): m_keyStates[RIGHT_ARROW_KEY_UP] = true; m_keyStates[RIGHT_ARROW_KEY_DOWN] = false; eventQ.push({ RIGHT_ARROW_KEY_UP }); break;
        case (SDLK_DOWN): m_keyStates[DOWN_ARROW_KEY_UP] = true; m_keyStates[DOWN_ARROW_KEY_DOWN] = false; eventQ.push({ DOWN_ARROW_KEY_UP }); break;
        case (SDLK_SPACE): m_keyStates[SPACE_KEY_UP] = true; m_keyStates[SPACE_KEY_DOWN] = false; eventQ.push({ SPACE_KEY_UP }); break;
        case (SDLK_MINUS): m_keyStates[MINUS_KEY_UP] = true; m_keyStates[MINUS_KEY_DOWN] = false; eventQ.push({ MINUS_KEY_UP }); break;
        case (SDLK_BACKSPACE): m_keyStates[BACKSPACE_KEY_UP] = true; m_keyStates[BACKSPACE_KEY_DOWN] = false; eventQ.push({ BACKSPACE_KEY_UP }); break;
        default: break;
        }
    }

    void EventManager::dispatchMouseMoveEvent(SDL_MouseMotionEvent& mouseMotion)
    {
        m_mousePosition.first = mouseMotion.x;
        m_mousePosition.second = mouseMotion.y;
        eventQ.push({ MOUSE_MOVE, mouseMotion.x, mouseMotion.y });
    }

    void EventManager::dispatchMouseButtonDownEvent(SDL_MouseButtonEvent& mouseButtonEvent)
    {
        switch (mouseButtonEvent.button)
        {
        case (SDL_BUTTON_LEFT): m_mouseButtonStates[LEFT_MOUSE_BUTTON_DOWN] = true; m_mouseButtonStates[LEFT_MOUSE_BUTTON_UP] = false; eventQ.push({ LEFT_MOUSE_BUTTON_DOWN }); break;
        case (SDL_BUTTON_MIDDLE): m_mouseButtonStates[MIDDLE_MOUSE_BUTTON_DOWN] = true; m_mouseButtonStates[MIDDLE_MOUSE_BUTTON_UP] = false; eventQ.push({ MIDDLE_MOUSE_BUTTON_DOWN }); break;
        case (SDL_BUTTON_RIGHT): m_mouseButtonStates[RIGHT_MOUSE_BUTTON_DOWN] = true; m_mouseButtonStates[RIGHT_MOUSE_BUTTON_UP] = false; eventQ.push({ RIGHT_MOUSE_BUTTON_DOWN }); break;
        default: break;
        }
    }
    void EventManager::dispatchMouseButtonUpEvent(SDL_MouseButtonEvent& mouseButtonEvent)
    {
        switch (mouseButtonEvent.button)
        {
        case (SDL_BUTTON_LEFT): m_mouseButtonStates[LEFT_MOUSE_BUTTON_UP] = true; m_mouseButtonStates[LEFT_MOUSE_BUTTON_DOWN] = false; eventQ.push({ LEFT_MOUSE_BUTTON_UP }); break;
        case (SDL_BUTTON_MIDDLE): m_mouseButtonStates[MIDDLE_MOUSE_BUTTON_UP] = true; m_mouseButtonStates[MIDDLE_MOUSE_BUTTON_DOWN] = false; eventQ.push({ MIDDLE_MOUSE_BUTTON_UP }); break;
        case (SDL_BUTTON_RIGHT): m_mouseButtonStates[RIGHT_MOUSE_BUTTON_UP] = true; m_mouseButtonStates[RIGHT_MOUSE_BUTTON_DOWN] = false; eventQ.push({ RIGHT_MOUSE_BUTTON_UP }); break;
        default: break;
        }
    }
    void EventManager::dispatchMouseScrollEvent(SDL_MouseWheelEvent& mouseWheelEvent)
    {
        m_scrollDirection.first = mouseWheelEvent.x;
        m_scrollDirection.second = mouseWheelEvent.y;
        switch (mouseWheelEvent.direction)
        {
        case (SDL_MOUSEWHEEL_NORMAL): eventQ.push({ MOUSE_WHEEL_MOVED, mouseWheelEvent.x, mouseWheelEvent.y }); break;
        default: break;
        }
    }
}