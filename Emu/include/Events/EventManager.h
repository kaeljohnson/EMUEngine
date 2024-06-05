#pragma once

#include <queue>
#include <unordered_map>
#include <utility>

#include "../Core.h"
#include "../SDLWrapper/SDLWrapper.h"
#include "../Events/Event.h"

namespace Engine
{
    class EventManager
    {
    public:
        std::queue<Event> eventQ;

    public:
        EMU_API static EventManager* GetInstance();
        EMU_API inline const std::unordered_map<EventType, bool>& GetKeyStates() const { return m_keyStates; }
        EMU_API inline const std::unordered_map<EventType, bool>& GetMouseButtonStates() const { return m_mouseButtonStates; }
        EMU_API inline const std::pair<int, int>& GetMousePosition() const { return m_mousePosition; }
        EMU_API inline const std::pair<int, int>& GetScrollDirection() const { return m_scrollDirection; }

    public:
        ~EventManager() = default;

        void HandleEvents();

        EventManager(const EventManager&) = delete;
        EventManager& operator=(const EventManager&) = delete;
        EventManager(EventManager&&) = delete;
        EventManager& operator=(EventManager&&) = delete;

    private:
        static EventManager* instance;
        EventManager();

        std::unordered_map<EventType, bool> m_keyStates;
        std::unordered_map<EventType, bool> m_mouseButtonStates;
        std::pair<int, int> m_mousePosition;
        std::pair<int, int> m_scrollDirection;

        void dispatchQuitEvent();
        void dispatchWindowEvent(SDL_WindowEvent& windowEvent);
        void dispatchKeydownEvent(SDL_Keycode& keyCode);
        void dispatchKeyupEvent(SDL_Keycode& keyCode);
        void dispatchMouseMoveEvent(SDL_MouseMotionEvent& mouseMotion);
        void dispatchMouseButtonDownEvent(SDL_MouseButtonEvent& mouseButtonEvent);
        void dispatchMouseButtonUpEvent(SDL_MouseButtonEvent& mouseButtonEvent);
        void dispatchMouseScrollEvent(SDL_MouseWheelEvent& mouseWheelEvent);
    };
}