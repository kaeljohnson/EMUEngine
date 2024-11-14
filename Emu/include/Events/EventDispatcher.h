#pragma once

#include <queue>
#include <unordered_map>

#include "../MathUtil.h"
#include "Event.h"

#include "KeyStates.h"
#include "MouseStates.h"
#include "../ISDL/ISDL.h"

namespace Engine
{
	class EventDispatcher
	{
    public:
        EventDispatcher(std::queue<Event>& eventQ);
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
        Vector2D<int>& refMousePosition;
        Vector2D<int>& refScrollDirection;
	};
}