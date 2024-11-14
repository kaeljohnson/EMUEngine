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

    class EventManager
    {
    public:
        EMU_API void RegisterEventHandler(EventType type, EventHandler handler);
        

    public:
        EventManager();
        ~EventManager() = default;

        void HandleEvents();
        void ProcessEvents();

        EventManager(const EventManager&) = delete;
        EventManager& operator=(const EventManager&) = delete;
        EventManager(EventManager&&) = delete;
        EventManager& operator=(EventManager&&) = delete;

    private:
        EventQueue m_eventQ;
        EventHandlerMap m_eventHandlers;

        

        std::unique_ptr<EventDispatcher> m_eventDispatcher;
    };
}