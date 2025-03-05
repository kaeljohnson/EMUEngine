#pragma once

#include "../../include/ISDL/ISDL.h"
#include "../../include/Events/IOEventDispatcher.h"

namespace Engine
{
    IOEventDispatcher::IOEventDispatcher(std::queue<IOEvent>& eventQ)
        : refEventQ(eventQ), refKeyStates(KeyStates::m_keyStates), refMouseButtonStates(MouseStates::m_mouseStates), 
        refMousePosition(MouseStates::m_mousePosition), refScrollDirection(MouseStates::m_scrollDirection)
    {}

    void IOEventDispatcher::PollEvents()
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

        while (ISDL::PollEvent(&e))
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

    void IOEventDispatcher::dispatchQuitEvent()
    {
        refEventQ.push({ QUIT });
    }

    void IOEventDispatcher::dispatchWindowEvent(SDL_WindowEvent& windowEvent)
    {
        switch (windowEvent.event)
        {
        case (SDL_WINDOWEVENT_RESIZED): refEventQ.push({ RESIZE_WINDOW, windowEvent.data1, windowEvent.data2 }); break;
        default: break;
        }
    }

    void IOEventDispatcher::dispatchKeydownEvent(SDL_Keycode& keyCode)
    {
        switch (keyCode)
        {
        case (SDLK_ESCAPE): refKeyStates[ESCAPE_KEY_DOWN] = true; refKeyStates[ESCAPE_KEY_UP] = false; refEventQ.push({ ESCAPE_KEY_DOWN }); break;
        case (SDLK_EQUALS): refKeyStates[EQUALS_KEY_DOWN] = true; refKeyStates[EQUALS_KEY_UP] = false; refEventQ.push({ EQUALS_KEY_DOWN }); break;
        case (SDLK_0): refKeyStates[ZERO_KEY_DOWN] = true; refKeyStates[ZERO_KEY_UP] = false; refEventQ.push({ ZERO_KEY_DOWN }); break;
        case (SDLK_1): refKeyStates[ONE_KEY_DOWN] = true; refKeyStates[ONE_KEY_UP] = false; refEventQ.push({ ONE_KEY_DOWN }); break;
        case (SDLK_2): refKeyStates[TWO_KEY_DOWN] = true; refKeyStates[TWO_KEY_UP] = false; refEventQ.push({ TWO_KEY_DOWN }); break;
        case (SDLK_3): refKeyStates[THREE_KEY_DOWN] = true; refKeyStates[THREE_KEY_UP] = false; refEventQ.push({ THREE_KEY_DOWN }); break;
        case (SDLK_4): refKeyStates[FOUR_KEY_DOWN] = true; refKeyStates[FOUR_KEY_UP] = false; refEventQ.push({ FOUR_KEY_DOWN }); break;
        case (SDLK_5): refKeyStates[FIVE_KEY_DOWN] = true; refKeyStates[FIVE_KEY_UP] = false; refEventQ.push({ FIVE_KEY_DOWN }); break;
        case (SDLK_6): refKeyStates[SIX_KEY_DOWN] = true; refKeyStates[SIX_KEY_UP] = false; refEventQ.push({ SIX_KEY_DOWN }); break;
        case (SDLK_7): refKeyStates[SEVEN_KEY_DOWN] = true; refKeyStates[SEVEN_KEY_UP] = false; refEventQ.push({ SEVEN_KEY_DOWN }); break;
        case (SDLK_8): refKeyStates[EIGHT_KEY_DOWN] = true; refKeyStates[EIGHT_KEY_UP] = false; refEventQ.push({ EIGHT_KEY_DOWN }); break;
        case (SDLK_9): refKeyStates[NINE_KEY_DOWN] = true; refKeyStates[NINE_KEY_UP] = false; refEventQ.push({ NINE_KEY_DOWN }); break;
        case (SDLK_q): refKeyStates[Q_KEY_DOWN] = true; refKeyStates[Q_KEY_UP] = false; refEventQ.push({ Q_KEY_DOWN }); break;
        case (SDLK_w): refKeyStates[W_KEY_DOWN] = true; refKeyStates[W_KEY_UP] = false; refEventQ.push({ W_KEY_DOWN }); break;
        case (SDLK_e): refKeyStates[E_KEY_DOWN] = true; refKeyStates[E_KEY_UP] = false; refEventQ.push({ E_KEY_DOWN }); break;
        case (SDLK_r): refKeyStates[R_KEY_DOWN] = true; refKeyStates[R_KEY_UP] = false; refEventQ.push({ R_KEY_DOWN }); break;
        case (SDLK_t): refKeyStates[T_KEY_DOWN] = true; refKeyStates[T_KEY_UP] = false; refEventQ.push({ T_KEY_DOWN }); break;
        case (SDLK_y): refKeyStates[Y_KEY_DOWN] = true; refKeyStates[Y_KEY_UP] = false; refEventQ.push({ Y_KEY_DOWN }); break;
        case (SDLK_u): refKeyStates[U_KEY_DOWN] = true; refKeyStates[U_KEY_UP] = false; refEventQ.push({ U_KEY_DOWN }); break;
        case (SDLK_i): refKeyStates[I_KEY_DOWN] = true; refKeyStates[I_KEY_UP] = false; refEventQ.push({ I_KEY_DOWN }); break;
        case (SDLK_o): refKeyStates[O_KEY_DOWN] = true; refKeyStates[O_KEY_UP] = false; refEventQ.push({ O_KEY_DOWN }); break;
        case (SDLK_p): refKeyStates[P_KEY_DOWN] = true; refKeyStates[P_KEY_UP] = false; refEventQ.push({ P_KEY_DOWN }); break;
        case (SDLK_a): refKeyStates[A_KEY_DOWN] = true; refKeyStates[A_KEY_UP] = false; refEventQ.push({ A_KEY_DOWN }); break;
        case (SDLK_s): refKeyStates[S_KEY_DOWN] = true; refKeyStates[S_KEY_UP] = false; refEventQ.push({ S_KEY_DOWN }); break;
        case (SDLK_d): refKeyStates[D_KEY_DOWN] = true; refKeyStates[D_KEY_UP] = false; refEventQ.push({ D_KEY_DOWN }); break;
        case (SDLK_f): refKeyStates[F_KEY_DOWN] = true; refKeyStates[F_KEY_UP] = false; refEventQ.push({ F_KEY_DOWN }); break;
        case (SDLK_g): refKeyStates[G_KEY_DOWN] = true; refKeyStates[G_KEY_UP] = false; refEventQ.push({ G_KEY_DOWN }); break;
        case (SDLK_h): refKeyStates[H_KEY_DOWN] = true; refKeyStates[H_KEY_UP] = false; refEventQ.push({ H_KEY_DOWN }); break;
        case (SDLK_j): refKeyStates[J_KEY_DOWN] = true; refKeyStates[J_KEY_UP] = false; refEventQ.push({ J_KEY_DOWN }); break;
        case (SDLK_k): refKeyStates[K_KEY_DOWN] = true; refKeyStates[K_KEY_UP] = false; refEventQ.push({ K_KEY_DOWN }); break;
        case (SDLK_l): refKeyStates[L_KEY_DOWN] = true; refKeyStates[L_KEY_UP] = false; refEventQ.push({ L_KEY_DOWN }); break;
        case (SDLK_z): refKeyStates[Z_KEY_DOWN] = true; refKeyStates[Z_KEY_UP] = false; refEventQ.push({ Z_KEY_DOWN }); break;
        case (SDLK_x): refKeyStates[X_KEY_DOWN] = true; refKeyStates[X_KEY_UP] = false; refEventQ.push({ X_KEY_DOWN }); break;
        case (SDLK_c): refKeyStates[C_KEY_DOWN] = true; refKeyStates[C_KEY_UP] = false; refEventQ.push({ C_KEY_DOWN }); break;
        case (SDLK_v): refKeyStates[V_KEY_DOWN] = true; refKeyStates[V_KEY_UP] = false; refEventQ.push({ V_KEY_DOWN }); break;
        case (SDLK_b): refKeyStates[B_KEY_DOWN] = true; refKeyStates[B_KEY_UP] = false; refEventQ.push({ B_KEY_DOWN }); break;
        case (SDLK_n): refKeyStates[N_KEY_DOWN] = true; refKeyStates[N_KEY_UP] = false; refEventQ.push({ N_KEY_DOWN }); break;
        case (SDLK_m): refKeyStates[M_KEY_DOWN] = true; refKeyStates[M_KEY_UP] = false; refEventQ.push({ M_KEY_DOWN }); break;
        case (SDLK_TAB): refKeyStates[TAB_KEY_DOWN] = true; refKeyStates[TAB_KEY_UP] = false; refEventQ.push({ TAB_KEY_DOWN }); break;
        case (SDLK_CAPSLOCK): refKeyStates[CAPS_LOCK_KEY_DOWN] = true; refKeyStates[CAPS_LOCK_KEY_UP] = false; refEventQ.push({ CAPS_LOCK_KEY_DOWN }); break;
        case (SDLK_LSHIFT): refKeyStates[LEFT_SHIFT_KEY_DOWN] = true; refKeyStates[LEFT_SHIFT_KEY_UP] = false; refEventQ.push({ LEFT_SHIFT_KEY_DOWN }); break;
        case (SDLK_RSHIFT): refKeyStates[RIGHT_SHIFT_KEY_DOWN] = true; refKeyStates[RIGHT_SHIFT_KEY_UP] = false; refEventQ.push({ RIGHT_SHIFT_KEY_DOWN }); break;
        case (SDLK_RETURN): refKeyStates[ENTER_KEY_DOWN] = true; refKeyStates[ENTER_KEY_UP] = false; refEventQ.push({ ENTER_KEY_DOWN }); break;
        case (SDLK_LEFT): refKeyStates[LEFT_ARROW_KEY_DOWN] = true; refKeyStates[LEFT_ARROW_KEY_UP] = false; refEventQ.push({ LEFT_ARROW_KEY_DOWN }); break;
        case (SDLK_UP): refKeyStates[UP_ARROW_KEY_DOWN] = true; refKeyStates[UP_ARROW_KEY_UP] = false; refEventQ.push({ UP_ARROW_KEY_DOWN }); break;
        case (SDLK_RIGHT): refKeyStates[RIGHT_ARROW_KEY_DOWN] = true; refKeyStates[RIGHT_ARROW_KEY_UP] = false; refEventQ.push({ RIGHT_ARROW_KEY_DOWN }); break;
        case (SDLK_DOWN): refKeyStates[DOWN_ARROW_KEY_DOWN] = true; refKeyStates[DOWN_ARROW_KEY_UP] = false; refEventQ.push({ DOWN_ARROW_KEY_DOWN }); break;
        case (SDLK_SPACE): refKeyStates[SPACE_KEY_DOWN] = true; refKeyStates[SPACE_KEY_UP] = false; refEventQ.push({ SPACE_KEY_DOWN }); break;
        case (SDLK_MINUS): refKeyStates[MINUS_KEY_DOWN] = true; refKeyStates[MINUS_KEY_UP] = false; refEventQ.push({ MINUS_KEY_DOWN }); break;
        case (SDLK_BACKSPACE): refKeyStates[BACKSPACE_KEY_DOWN] = true; refKeyStates[BACKSPACE_KEY_UP] = false; refEventQ.push({ BACKSPACE_KEY_DOWN }); break;
        default: break;
        }
    }

    void IOEventDispatcher::dispatchKeyupEvent(SDL_Keycode& keyCode)
    {
        switch (keyCode)
        {
        case (SDLK_ESCAPE): refKeyStates[ESCAPE_KEY_UP] = true; refKeyStates[ESCAPE_KEY_DOWN] = false; refEventQ.push({ ESCAPE_KEY_UP }); break;
        case (SDLK_EQUALS): refKeyStates[EQUALS_KEY_UP] = true; refKeyStates[EQUALS_KEY_DOWN] = false; refEventQ.push({ EQUALS_KEY_UP }); break;
        case (SDLK_0): refKeyStates[ZERO_KEY_UP] = true; refKeyStates[ZERO_KEY_DOWN] = false; refEventQ.push({ ZERO_KEY_UP }); break;
        case (SDLK_1): refKeyStates[ONE_KEY_UP] = true; refKeyStates[ONE_KEY_DOWN] = false; refEventQ.push({ ONE_KEY_UP }); break;
        case (SDLK_2): refKeyStates[TWO_KEY_UP] = true; refKeyStates[TWO_KEY_DOWN] = false; refEventQ.push({ TWO_KEY_UP }); break;
        case (SDLK_3): refKeyStates[THREE_KEY_UP] = true; refKeyStates[THREE_KEY_DOWN] = false; refEventQ.push({ THREE_KEY_UP }); break;
        case (SDLK_4): refKeyStates[FOUR_KEY_UP] = true; refKeyStates[FOUR_KEY_DOWN] = false; refEventQ.push({ FOUR_KEY_UP }); break;
        case (SDLK_5): refKeyStates[FIVE_KEY_UP] = true; refKeyStates[FIVE_KEY_DOWN] = false; refEventQ.push({ FIVE_KEY_UP }); break;
        case (SDLK_6): refKeyStates[SIX_KEY_UP] = true; refKeyStates[SIX_KEY_DOWN] = false; refEventQ.push({ SIX_KEY_UP }); break;
        case (SDLK_7): refKeyStates[SEVEN_KEY_UP] = true; refKeyStates[SEVEN_KEY_DOWN] = false; refEventQ.push({ SEVEN_KEY_UP }); break;
        case (SDLK_8): refKeyStates[EIGHT_KEY_UP] = true; refKeyStates[EIGHT_KEY_DOWN] = false; refEventQ.push({ EIGHT_KEY_UP }); break;
        case (SDLK_9): refKeyStates[NINE_KEY_UP] = true; refKeyStates[NINE_KEY_DOWN] = false; refEventQ.push({ NINE_KEY_UP }); break;
        case (SDLK_q): refKeyStates[Q_KEY_UP] = true; refKeyStates[Q_KEY_DOWN] = false; refEventQ.push({ Q_KEY_UP }); break;
        case (SDLK_w): refKeyStates[W_KEY_UP] = true; refKeyStates[W_KEY_DOWN] = false; refEventQ.push({ W_KEY_UP }); break;
        case (SDLK_e): refKeyStates[E_KEY_UP] = true; refKeyStates[E_KEY_DOWN] = false; refEventQ.push({ E_KEY_UP }); break;
        case (SDLK_r): refKeyStates[R_KEY_UP] = true; refKeyStates[R_KEY_DOWN] = false; refEventQ.push({ R_KEY_UP }); break;
        case (SDLK_t): refKeyStates[T_KEY_UP] = true; refKeyStates[T_KEY_DOWN] = false; refEventQ.push({ T_KEY_UP }); break;
        case (SDLK_y): refKeyStates[Y_KEY_UP] = true; refKeyStates[Y_KEY_DOWN] = false; refEventQ.push({ Y_KEY_UP }); break;
        case (SDLK_u): refKeyStates[U_KEY_UP] = true; refKeyStates[U_KEY_DOWN] = false; refEventQ.push({ U_KEY_UP }); break;
        case (SDLK_i): refKeyStates[I_KEY_UP] = true; refKeyStates[I_KEY_DOWN] = false; refEventQ.push({ I_KEY_UP }); break;
        case (SDLK_o): refKeyStates[O_KEY_UP] = true; refKeyStates[O_KEY_DOWN] = false; refEventQ.push({ O_KEY_UP }); break;
        case (SDLK_p): refKeyStates[P_KEY_UP] = true; refKeyStates[P_KEY_DOWN] = false; refEventQ.push({ P_KEY_UP }); break;
        case (SDLK_a): refKeyStates[A_KEY_UP] = true; refKeyStates[A_KEY_DOWN] = false; refEventQ.push({ A_KEY_UP }); break;
        case (SDLK_s): refKeyStates[S_KEY_UP] = true; refKeyStates[S_KEY_DOWN] = false; refEventQ.push({ S_KEY_UP }); break;
        case (SDLK_d): refKeyStates[D_KEY_UP] = true; refKeyStates[D_KEY_DOWN] = false; refEventQ.push({ D_KEY_UP }); break;
        case (SDLK_f): refKeyStates[F_KEY_UP] = true; refKeyStates[F_KEY_DOWN] = false; refEventQ.push({ F_KEY_UP }); break;
        case (SDLK_g): refKeyStates[G_KEY_UP] = true; refKeyStates[G_KEY_DOWN] = false; refEventQ.push({ G_KEY_UP }); break;
        case (SDLK_h): refKeyStates[H_KEY_UP] = true; refKeyStates[H_KEY_DOWN] = false; refEventQ.push({ H_KEY_UP }); break;
        case (SDLK_j): refKeyStates[J_KEY_UP] = true; refKeyStates[J_KEY_DOWN] = false; refEventQ.push({ J_KEY_UP }); break;
        case (SDLK_k): refKeyStates[K_KEY_UP] = true; refKeyStates[K_KEY_DOWN] = false; refEventQ.push({ K_KEY_UP }); break;
        case (SDLK_l): refKeyStates[L_KEY_UP] = true; refKeyStates[L_KEY_DOWN] = false; refEventQ.push({ L_KEY_UP }); break;
        case (SDLK_z): refKeyStates[Z_KEY_UP] = true; refKeyStates[Z_KEY_DOWN] = false; refEventQ.push({ Z_KEY_UP }); break;
        case (SDLK_x): refKeyStates[X_KEY_UP] = true; refKeyStates[X_KEY_DOWN] = false; refEventQ.push({ X_KEY_UP }); break;
        case (SDLK_c): refKeyStates[C_KEY_UP] = true; refKeyStates[C_KEY_DOWN] = false; refEventQ.push({ C_KEY_UP }); break;
        case (SDLK_v): refKeyStates[V_KEY_UP] = true; refKeyStates[V_KEY_DOWN] = false; refEventQ.push({ V_KEY_UP }); break;
        case (SDLK_b): refKeyStates[B_KEY_UP] = true; refKeyStates[B_KEY_DOWN] = false; refEventQ.push({ B_KEY_UP }); break;
        case (SDLK_n): refKeyStates[N_KEY_UP] = true; refKeyStates[N_KEY_DOWN] = false; refEventQ.push({ N_KEY_UP }); break;
        case (SDLK_m): refKeyStates[M_KEY_UP] = true; refKeyStates[M_KEY_DOWN] = false; refEventQ.push({ M_KEY_UP }); break;
        case (SDLK_TAB): refKeyStates[TAB_KEY_UP] = true; refKeyStates[TAB_KEY_DOWN] = false; refEventQ.push({ TAB_KEY_UP }); break;
        case (SDLK_CAPSLOCK): refKeyStates[CAPS_LOCK_KEY_UP] = true; refKeyStates[CAPS_LOCK_KEY_DOWN] = false; refEventQ.push({ CAPS_LOCK_KEY_UP }); break;
        case (SDLK_LSHIFT): refKeyStates[LEFT_SHIFT_KEY_UP] = true; refKeyStates[LEFT_SHIFT_KEY_DOWN] = false; refEventQ.push({ LEFT_SHIFT_KEY_UP }); break;
        case (SDLK_RSHIFT): refKeyStates[RIGHT_SHIFT_KEY_UP] = true; refKeyStates[RIGHT_SHIFT_KEY_DOWN] = false; refEventQ.push({ RIGHT_SHIFT_KEY_UP }); break;
        case (SDLK_RETURN): refKeyStates[ENTER_KEY_UP] = true; refKeyStates[ENTER_KEY_DOWN] = false; refEventQ.push({ ENTER_KEY_UP }); break;
        case (SDLK_LEFT): refKeyStates[LEFT_ARROW_KEY_UP] = true; refKeyStates[LEFT_ARROW_KEY_DOWN] = false; refEventQ.push({ LEFT_ARROW_KEY_UP }); break;
        case (SDLK_UP): refKeyStates[UP_ARROW_KEY_UP] = true; refKeyStates[UP_ARROW_KEY_DOWN] = false; refEventQ.push({ UP_ARROW_KEY_UP }); break;
        case (SDLK_RIGHT): refKeyStates[RIGHT_ARROW_KEY_UP] = true; refKeyStates[RIGHT_ARROW_KEY_DOWN] = false; refEventQ.push({ RIGHT_ARROW_KEY_UP }); break;
        case (SDLK_DOWN): refKeyStates[DOWN_ARROW_KEY_UP] = true; refKeyStates[DOWN_ARROW_KEY_DOWN] = false; refEventQ.push({ DOWN_ARROW_KEY_UP }); break;
        case (SDLK_SPACE): refKeyStates[SPACE_KEY_UP] = true; refKeyStates[SPACE_KEY_DOWN] = false; refEventQ.push({ SPACE_KEY_UP }); break;
        case (SDLK_MINUS): refKeyStates[MINUS_KEY_UP] = true; refKeyStates[MINUS_KEY_DOWN] = false; refEventQ.push({ MINUS_KEY_UP }); break;
        case (SDLK_BACKSPACE): refKeyStates[BACKSPACE_KEY_UP] = true; refKeyStates[BACKSPACE_KEY_DOWN] = false; refEventQ.push({ BACKSPACE_KEY_UP }); break;
        default: break;
        }
    }

    void IOEventDispatcher::dispatchMouseMoveEvent(SDL_MouseMotionEvent& mouseMotion)
    {
        refMousePosition.X = mouseMotion.x;
        refMousePosition.Y = mouseMotion.y;
        refEventQ.push({ MOUSE_MOVE, mouseMotion.x, mouseMotion.y });
    }

    void IOEventDispatcher::dispatchMouseButtonDownEvent(SDL_MouseButtonEvent& mouseButtonEvent)
    {
        switch (mouseButtonEvent.button)
        {
        case (SDL_BUTTON_LEFT): refMouseButtonStates[LEFT_MOUSE_BUTTON_DOWN] = true; refMouseButtonStates[LEFT_MOUSE_BUTTON_UP] = false; refEventQ.push({ LEFT_MOUSE_BUTTON_DOWN }); break;
        case (SDL_BUTTON_MIDDLE): refMouseButtonStates[MIDDLE_MOUSE_BUTTON_DOWN] = true; refMouseButtonStates[MIDDLE_MOUSE_BUTTON_UP] = false; refEventQ.push({ MIDDLE_MOUSE_BUTTON_DOWN }); break;
        case (SDL_BUTTON_RIGHT): refMouseButtonStates[RIGHT_MOUSE_BUTTON_DOWN] = true; refMouseButtonStates[RIGHT_MOUSE_BUTTON_UP] = false; refEventQ.push({ RIGHT_MOUSE_BUTTON_DOWN }); break;
        default: break;
        }
    }
    void IOEventDispatcher::dispatchMouseButtonUpEvent(SDL_MouseButtonEvent& mouseButtonEvent)
    {
        switch (mouseButtonEvent.button)
        {
        case (SDL_BUTTON_LEFT): refMouseButtonStates[LEFT_MOUSE_BUTTON_UP] = true; refMouseButtonStates[LEFT_MOUSE_BUTTON_DOWN] = false; refEventQ.push({ LEFT_MOUSE_BUTTON_UP }); break;
        case (SDL_BUTTON_MIDDLE): refMouseButtonStates[MIDDLE_MOUSE_BUTTON_UP] = true; refMouseButtonStates[MIDDLE_MOUSE_BUTTON_DOWN] = false; refEventQ.push({ MIDDLE_MOUSE_BUTTON_UP }); break;
        case (SDL_BUTTON_RIGHT): refMouseButtonStates[RIGHT_MOUSE_BUTTON_UP] = true; refMouseButtonStates[RIGHT_MOUSE_BUTTON_DOWN] = false; refEventQ.push({ RIGHT_MOUSE_BUTTON_UP }); break;
        default: break;
        }
    }
    void IOEventDispatcher::dispatchMouseScrollEvent(SDL_MouseWheelEvent& mouseWheelEvent)
    {
        refScrollDirection.X = mouseWheelEvent.x;
        refScrollDirection.Y = mouseWheelEvent.y;
        switch (mouseWheelEvent.direction)
        {
        case (SDL_MOUSEWHEEL_NORMAL): refEventQ.push({ MOUSE_WHEEL_MOVED, mouseWheelEvent.x, mouseWheelEvent.y }); break;
        default: break;
        }
    }
}