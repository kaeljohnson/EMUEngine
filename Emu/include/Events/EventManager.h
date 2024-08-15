#pragma once

#include <queue>
#include <unordered_map>
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
        EMU_API inline const EventStatesMap& GetKeyStates() const { return m_keyStates; }
        EMU_API inline const std::unordered_map<EventType, bool>& GetMouseButtonStates() const { return m_mouseButtonStates; }
        EMU_API inline const Math::Vector2D<int>& GetMousePosition() const { return m_mousePosition; }
        EMU_API inline const Math::Vector2D<int>& GetScrollDirection() const { return m_scrollDirection; }

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
        EventStatesMap m_keyStates;
        std::unordered_map<EventType, bool> m_mouseButtonStates;

        Math::Vector2D<int> m_mousePosition;
        Math::Vector2D<int> m_scrollDirection;

        std::unique_ptr<EventDispatcher> m_eventDispatcher;
    };
}