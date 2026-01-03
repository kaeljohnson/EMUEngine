#pragma once

#include "../Events/IOEvent.h"
#include "../Includes.h"
#include "../MathUtil.h"

namespace Engine
{
    using IOEventQueue = std::queue<IOEvent>;
    using IOEventHandler = std::function<void(IOEvent&)>;
    using IOEventHandlerMap = std::unordered_map<IOEventType, IOEventHandler>;
    using IOEventStatesMap = std::unordered_map<IOEventType, bool>;

    /**
	* @brief System to manage IO events, including registration of event listeners and processing of events.
    */
    class IOEventSystem
    {
    public:
        void RegisterIOEventListener(IOEventType type, IOEventHandler handler);
        
    public:

		IOEventSystem();

		void Initialize();

        void HandleEvents();
        void ProcessEvents();

    private:
        IOEventQueue m_eventQ;

		// move to Event listener class
        IOEventHandlerMap m_ioEventListenerMap;
    };
}