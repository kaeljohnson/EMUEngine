#pragma once

#include <queue>
#include <unordered_map>
#include <utility>
#include <memory>
#include <functional>

#include "../Core.h"
#include "../MathUtil.h"

#include "../Events/Event.h"

namespace Engine
{
    class EventDispatcher;

    using EventQueue = std::queue<Event>;
    using EventHandler = std::function<void(Event&)>;
    using EventHandlerMap = std::unordered_map<EventType, EventHandler>;
    using EventStatesMap = std::unordered_map<EventType, bool>;

    class IOEventSystem
    {
    public:
        EMU_API static void RegisterEventHandler(EventType type, EventHandler handler);
        
    public:

		static void Initialize();

        static void HandleEvents();
        static void ProcessEvents();

    private:
        static EventQueue m_eventQ;

		// move to Event listener class
        static EventHandlerMap m_eventHandlers;

        static std::unique_ptr<EventDispatcher> m_eventDispatcher;
    };
}