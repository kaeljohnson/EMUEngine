#pragma once

#include <queue>
#include <memory>

#include "../../include/SDLWrapper/SDLWrapper.h"
#include "../../include/Events/EventManager.h"
#include "../../include/Events/EventDispatcher.h"
#include "../../include/Events/Event.h"

namespace Engine
{
    EventManager::EventManager() : m_eventDispatcher(std::make_unique<EventDispatcher>(m_eventQ, m_keyStates, m_mouseButtonStates, m_mousePosition, m_scrollDirection))
    {
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

    void EventManager::RegisterEventHandler(EventType type, EventHandler handler)
    {
		m_eventHandlers[type] = handler;
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

        m_eventDispatcher->PollEvents();
	}

    void EventManager::ProcessEvents()
    {
		/*
        	Process all events in the event queue. This function is called after 
            all events have been polled and pushed to the event queue. The events 
            are processed in the order they were pushed to the event queue. The 
            events are then dispatched to their respective event handlers.
        */

        while (!m_eventQ.empty())
        {
            Event& currentEvent = m_eventQ.front();

            if (m_eventHandlers.find(currentEvent.Type) != m_eventHandlers.end())
            {
                m_eventHandlers[currentEvent.Type](currentEvent);
            }

            // if (!currentEvent.Handled)
            // {
            //    ENGINE_TRACE_D("Unhandled Event: " + std::to_string(static_cast<int>(currentEvent.Type)));
            // }

            m_eventQ.pop();
        }
	}
}