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

    using EventHandler = std::function<void(Event&)>;
    using EventHandlerMap = std::unordered_map<EventType, EventHandler>;

    class EventManager
    {
    public:
        
        EventHandlerMap m_eventHandlers;

    public:
        EventManager();

        EMU_API inline const std::unordered_map<EventType, bool>& GetKeyStates() const { return m_keyStates; }
        EMU_API inline const std::unordered_map<EventType, bool>& GetMouseButtonStates() const { return m_mouseButtonStates; }
        EMU_API inline const Vector2D<int>& GetMousePosition() const { return m_mousePosition; }
        EMU_API inline const Vector2D<int>& GetScrollDirection() const { return m_scrollDirection; }

    public:
        ~EventManager() = default;

        void HandleEvents();
        void ProcessEvents();

        EventManager(const EventManager&) = delete;
        EventManager& operator=(const EventManager&) = delete;
        EventManager(EventManager&&) = delete;
        EventManager& operator=(EventManager&&) = delete;

    private:
        std::queue<Event> m_eventQ;
        std::unordered_map<EventType, bool> m_keyStates;
        std::unordered_map<EventType, bool> m_mouseButtonStates;

        Vector2D<int> m_mousePosition;
        Vector2D<int> m_scrollDirection;

        std::unique_ptr<EventDispatcher> m_eventDispatcher;
    };
}