#pragma once

#include "../../Public/EMUIOEvent.h"
#include "../../Public/Includes.h"
#include "../../Public/MathUtil.h"

namespace Engine
{
    /**
	* @brief System to manage IO events, including registration of event listeners and processing of events.
    */
    class IOEventSystem
    {
    public:

        IOEventSystem();

		/**
		* @brief Registers an event listener for a specific IO event type.
        * 
		* * @param type The type of the IO event to listen for.
		* * @param handler The handler function to be called when the event occurs.
		*/
        void RegisterIOEventListener(IOEventType type, IOEventHandler handler);

        /**
        * @brief Unregisters an event listener for a specific IO event type.
        * This is useful for when an action is dependent on other events occuring and then not ocurring, such
        * as a player walking by a door. This functionality needs to be available only when the player is in front of
        * the door.
        * 
        */
        void UnRegisterIOEventListener(IOEventType type);

        /**
		* @brief Initializes the IO event system.
        */
		void Initialize();

        /**
		* @brief Handles incoming IO events.
        *
        */
        void HandleEvents();

		/**
		* @brief Processes all events in the event queue.
		*/
        void ProcessEvents();

        /**
        * @brief Clear the handler map. Client may want to call this at the end of 
        * the runtime context.
        */
        void ClearAllHandlers();

        /**
        * @brief Clear the eventQ. Since the eventQ is processed each frame it is likely to be empty anyway. But if 
        * there are any lingering events the client may want to clear them between runtime contexts.
        */
        void ClearEventQ();



    private:
        IOEventQueue m_eventQ;

		// move to Event listener class
        IOEventHandlerMap m_ioEventListenerMap;
    };
}