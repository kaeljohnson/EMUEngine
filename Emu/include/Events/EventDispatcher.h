#pragma once

#include <queue>
#include <unordered_map>

#include "../MathUtil.h"
#include "Event.h"

#include "../SDLWrapper/SDLWrapper.h"

namespace Engine
{
	class EventDispatcher
	{
    public:
        EventDispatcher(std::queue<Event>& eventQ, std::unordered_map<EventType, bool>& keyStates, 
            std::unordered_map<EventType, bool>& mouseButtonStates, Math::Vector2D<int>& mousePosition, Math::Vector2D<int>& scrollDirection);
        void PollEvents();
    private:
        void dispatchQuitEvent();
        void dispatchWindowEvent(SDL_WindowEvent& windowEvent);
        void dispatchKeydownEvent(SDL_Keycode& keyCode);
        void dispatchKeyupEvent(SDL_Keycode& keyCode);
        void dispatchMouseMoveEvent(SDL_MouseMotionEvent& mouseMotion);
        void dispatchMouseButtonDownEvent(SDL_MouseButtonEvent& mouseButtonEvent);
        void dispatchMouseButtonUpEvent(SDL_MouseButtonEvent& mouseButtonEvent);
        void dispatchMouseScrollEvent(SDL_MouseWheelEvent& mouseWheelEvent);

        std::queue<Event>& refEventQ;
        std::unordered_map<EventType, bool>& refKeyStates;
        std::unordered_map<EventType, bool>& refMouseButtonStates;
        Math::Vector2D<int>& refMousePosition;
        Math::Vector2D<int>& refScrollDirection;
	};
}