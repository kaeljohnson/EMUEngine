#pragma once

#include <queue>
#include <memory>

#include "../../include/ISDL/ISDL.h"
#include "../../include/Events/IOEventSystem.h"
#include "../../include/Events/IOEventDispatcher.h"
#include "../../include/Events/IOEvent.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Events/KeyStates.h"
#include "../../include/Events/MouseStates.h"

namespace Engine
{
    // Provide the actual definition of the static member variable
    KeyStatesMap KeyStates::m_keyStates;
	MouseStatesMap MouseStates::m_mouseStates;
    Vector2D<int> MouseStates::m_mousePosition;
	Vector2D<int> MouseStates::m_scrollDirection;

     IOEventQueue IOEventSystem::m_eventQ;

    // move to Event listener class
    IOEventHandlerMap IOEventSystem::m_ioEventListeners;

    std::unique_ptr<IOEventDispatcher> IOEventSystem::m_eventDispatcher;

    void IOEventSystem::Initialize()
    {
		m_eventDispatcher = std::make_unique<IOEventDispatcher>(m_eventQ);

        // Initialize all key down states to false
        KeyStates::m_keyStates[ESCAPE_KEY_DOWN] = false;
        KeyStates::m_keyStates[EQUALS_KEY_DOWN] = false;
        KeyStates::m_keyStates[ZERO_KEY_DOWN] = false;
        KeyStates::m_keyStates[ONE_KEY_DOWN] = false;
        KeyStates::m_keyStates[TWO_KEY_DOWN] = false;
        KeyStates::m_keyStates[THREE_KEY_DOWN] = false;
        KeyStates::m_keyStates[FOUR_KEY_DOWN] = false;
        KeyStates::m_keyStates[FIVE_KEY_DOWN] = false;
        KeyStates::m_keyStates[SIX_KEY_DOWN] = false;
        KeyStates::m_keyStates[SEVEN_KEY_DOWN] = false;
        KeyStates::m_keyStates[EIGHT_KEY_DOWN] = false;
        KeyStates::m_keyStates[NINE_KEY_DOWN] = false;
        KeyStates::m_keyStates[Q_KEY_DOWN] = false;
        KeyStates::m_keyStates[W_KEY_DOWN] = false;
        KeyStates::m_keyStates[E_KEY_DOWN] = false;
        KeyStates::m_keyStates[R_KEY_DOWN] = false;
        KeyStates::m_keyStates[T_KEY_DOWN] = false;
        KeyStates::m_keyStates[Y_KEY_DOWN] = false;
        KeyStates::m_keyStates[U_KEY_DOWN] = false;
        KeyStates::m_keyStates[I_KEY_DOWN] = false;
        KeyStates::m_keyStates[O_KEY_DOWN] = false;
        KeyStates::m_keyStates[P_KEY_DOWN] = false;
        KeyStates::m_keyStates[A_KEY_DOWN] = false;
        KeyStates::m_keyStates[S_KEY_DOWN] = false;
        KeyStates::m_keyStates[D_KEY_DOWN] = false;
        KeyStates::m_keyStates[F_KEY_DOWN] = false;
        KeyStates::m_keyStates[G_KEY_DOWN] = false;
        KeyStates::m_keyStates[H_KEY_DOWN] = false;
        KeyStates::m_keyStates[J_KEY_DOWN] = false;
        KeyStates::m_keyStates[K_KEY_DOWN] = false;
        KeyStates::m_keyStates[L_KEY_DOWN] = false;
        KeyStates::m_keyStates[Z_KEY_DOWN] = false;
        KeyStates::m_keyStates[X_KEY_DOWN] = false;
        KeyStates::m_keyStates[C_KEY_DOWN] = false;
        KeyStates::m_keyStates[V_KEY_DOWN] = false;
        KeyStates::m_keyStates[B_KEY_DOWN] = false;
        KeyStates::m_keyStates[N_KEY_DOWN] = false;
        KeyStates::m_keyStates[M_KEY_DOWN] = false;
        KeyStates::m_keyStates[TAB_KEY_DOWN] = false;
        KeyStates::m_keyStates[CAPS_LOCK_KEY_DOWN] = false;
        KeyStates::m_keyStates[LEFT_SHIFT_KEY_DOWN] = false;
        KeyStates::m_keyStates[RIGHT_SHIFT_KEY_DOWN] = false;
        KeyStates::m_keyStates[ENTER_KEY_DOWN] = false;
        KeyStates::m_keyStates[LEFT_ARROW_KEY_DOWN] = false;
        KeyStates::m_keyStates[UP_ARROW_KEY_DOWN] = false;
        KeyStates::m_keyStates[RIGHT_ARROW_KEY_DOWN] = false;
        KeyStates::m_keyStates[DOWN_ARROW_KEY_DOWN] = false;
        KeyStates::m_keyStates[SPACE_KEY_DOWN] = false;
        KeyStates::m_keyStates[MINUS_KEY_DOWN] = false;
        KeyStates::m_keyStates[BACKSPACE_KEY_DOWN] = false;

        // Initialize all key up states to true
        KeyStates::m_keyStates[ESCAPE_KEY_UP] = true;
        KeyStates::m_keyStates[EQUALS_KEY_UP] = true;
        KeyStates::m_keyStates[ZERO_KEY_UP] = true;
        KeyStates::m_keyStates[ONE_KEY_UP] = true;
        KeyStates::m_keyStates[TWO_KEY_UP] = true;
        KeyStates::m_keyStates[THREE_KEY_UP] = true;
        KeyStates::m_keyStates[FOUR_KEY_UP] = true;
        KeyStates::m_keyStates[FIVE_KEY_UP] = true;
        KeyStates::m_keyStates[SIX_KEY_UP] = true;
        KeyStates::m_keyStates[SEVEN_KEY_UP] = true;
        KeyStates::m_keyStates[EIGHT_KEY_UP] = true;
        KeyStates::m_keyStates[NINE_KEY_UP] = true;
        KeyStates::m_keyStates[Q_KEY_UP] = true;
        KeyStates::m_keyStates[W_KEY_UP] = true;
        KeyStates::m_keyStates[E_KEY_UP] = true;
        KeyStates::m_keyStates[R_KEY_UP] = true;
        KeyStates::m_keyStates[T_KEY_UP] = true;
        KeyStates::m_keyStates[Y_KEY_UP] = true;
        KeyStates::m_keyStates[U_KEY_UP] = true;
        KeyStates::m_keyStates[I_KEY_UP] = true;
        KeyStates::m_keyStates[O_KEY_UP] = true;
        KeyStates::m_keyStates[P_KEY_UP] = true;
        KeyStates::m_keyStates[A_KEY_UP] = true;
        KeyStates::m_keyStates[S_KEY_UP] = true;
        KeyStates::m_keyStates[D_KEY_UP] = true;
        KeyStates::m_keyStates[F_KEY_UP] = true;
        KeyStates::m_keyStates[G_KEY_UP] = true;
        KeyStates::m_keyStates[H_KEY_UP] = true;
        KeyStates::m_keyStates[J_KEY_UP] = true;
        KeyStates::m_keyStates[K_KEY_UP] = true;
        KeyStates::m_keyStates[L_KEY_UP] = true;
        KeyStates::m_keyStates[Z_KEY_UP] = true;
        KeyStates::m_keyStates[X_KEY_UP] = true;
        KeyStates::m_keyStates[C_KEY_UP] = true;
        KeyStates::m_keyStates[V_KEY_UP] = true;
        KeyStates::m_keyStates[B_KEY_UP] = true;
        KeyStates::m_keyStates[N_KEY_UP] = true;
        KeyStates::m_keyStates[M_KEY_UP] = true;
        KeyStates::m_keyStates[TAB_KEY_UP] = true;
        KeyStates::m_keyStates[CAPS_LOCK_KEY_UP] = true;
        KeyStates::m_keyStates[LEFT_SHIFT_KEY_UP] = true;
        KeyStates::m_keyStates[RIGHT_SHIFT_KEY_UP] = true;
        KeyStates::m_keyStates[ENTER_KEY_UP] = true;
        KeyStates::m_keyStates[LEFT_ARROW_KEY_UP] = true;
        KeyStates::m_keyStates[UP_ARROW_KEY_UP] = true;
        KeyStates::m_keyStates[RIGHT_ARROW_KEY_UP] = true;
        KeyStates::m_keyStates[DOWN_ARROW_KEY_UP] = true;
        KeyStates::m_keyStates[SPACE_KEY_UP] = true;
        KeyStates::m_keyStates[MINUS_KEY_UP] = true;
        KeyStates::m_keyStates[BACKSPACE_KEY_UP] = true;

		// Initialize all mouse down states to false
		MouseStates::m_mouseStates[MIDDLE_MOUSE_BUTTON_DOWN] = false;
		MouseStates::m_mouseStates[LEFT_MOUSE_BUTTON_DOWN] = false;
		MouseStates::m_mouseStates[RIGHT_MOUSE_BUTTON_DOWN] = false;

		// Initialize all mouse up states to true
		MouseStates::m_mouseStates[MIDDLE_MOUSE_BUTTON_UP] = true;
		MouseStates::m_mouseStates[LEFT_MOUSE_BUTTON_UP] = true;
		MouseStates::m_mouseStates[RIGHT_MOUSE_BUTTON_UP] = true;

    }

    void IOEventSystem::RegisterIOEventListener(IOEventType type, IOEventHandler handler)
    {
		m_ioEventListeners[type] = handler;
	}

	void IOEventSystem::HandleEvents()
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

        m_eventDispatcher->PollEvents();
	}

    void IOEventSystem::ProcessEvents()
    {
		/*
        	Process all events in the event queue. This function is called after 
            all events have been polled and pushed to the event queue. The events 
            are processed in the order they were pushed to the event queue. The 
            events are then dispatched to their respective event handlers.
        */

        while (!m_eventQ.empty())
        {
            IOEvent& currentEvent = m_eventQ.front();

            if (m_ioEventListeners.find(currentEvent.Type) != m_ioEventListeners.end())
            {
                m_ioEventListeners[currentEvent.Type](currentEvent);
            }

            // if (!currentEvent.Handled)
            // {
            //    ENGINE_TRACE_D("Unhandled Event: " + std::to_string(static_cast<int>(currentEvent.Type)));
            // }

            m_eventQ.pop();
        }
	}
}