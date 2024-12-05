#pragma once

#include <queue>
#include <unordered_map>
#include <utility>
#include <memory>
#include <functional>

#include "../Core.h"
#include "../MathUtil.h"

#include "../Events/IOEvent.h"

namespace Engine
{
    class IOEventDispatcher;

    using IOEventQueue = std::queue<IOEvent>;
    using IOEventHandler = std::function<void(IOEvent&)>;
    using IOEventHandlerMap = std::unordered_map<IOEventType, IOEventHandler>;
    using IOEventStatesMap = std::unordered_map<IOEventType, bool>;

    class IOEventSystem
    {
    public:
        EMU_API static void RegisterIOEventListener(IOEventType type, IOEventHandler handler);
        
    public:

		static void Initialize();

        static void HandleEvents();
        static void ProcessEvents();

    private:
        static IOEventQueue m_eventQ;

		// move to Event listener class
        static IOEventHandlerMap m_ioEventListeners;

        static std::unique_ptr<IOEventDispatcher> m_eventDispatcher;
    };
}