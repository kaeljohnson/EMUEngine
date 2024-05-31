#pragma once

#include <queue>
#include <unordered_map>
#include <utility>

#include "../Core.h"
#include "../SDLWrapper/SDLWrapper.h"
#include "../Events/Event.h"

namespace Engine
{
	// EventManager class. Polls for events and dispatches them to the event queue.
    class EventManager
    {
    private:
        static EventManager* instance;
        EventManager();

        std::unordered_map<EventType, bool> keyStates;
        std::unordered_map<EventType, bool> mouseButtonStates;
        std::pair<int, int> mousePosition;
        std::pair<int, int> scrollDirection;

        void dispatchQuitEvent();
        void dispatchWindowEvent(SDL_WindowEvent& windowEvent);
        void dispatchKeydownEvent(SDL_Keycode& keyCode);
        void dispatchKeyupEvent(SDL_Keycode& keyCode);
        void dispatchMouseMoveEvent(SDL_MouseMotionEvent& mouseMotion);
        void dispatchMouseButtonDownEvent(SDL_MouseButtonEvent& mouseButtonEvent);
        void dispatchMouseButtonUpEvent(SDL_MouseButtonEvent& mouseButtonEvent);
        void dispatchMouseScrollEvent(SDL_MouseWheelEvent& mouseWheelEvent);

    public:
        std::queue<Event> eventQ;
    public:
        EMU_API static EventManager* GetInstance();
        ~EventManager() = default;

        EMU_API const std::unordered_map<EventType, bool>& GetKeyStates() const;
        EMU_API const std::unordered_map<EventType, bool>& GetMouseButtonStates() const;
        EMU_API const std::pair<int, int>& GetMousePosition() const;
        EMU_API const std::pair<int, int>& GetScrollDirection() const;

        void HandleEvents();

        EventManager(const EventManager&) = delete;
        EventManager& operator=(const EventManager&) = delete;
        EventManager(EventManager&&) = delete;
        EventManager& operator=(EventManager&&) = delete;
    };
}