#pragma once

#include "../Includes.h"
#include "../ISDL/ISDL.h"
#include "../MathUtil.h"
#include "IOEvent.h"
#include "KeyStates.h"
#include "MouseStates.h"

namespace Engine
{
	class IOEventDispatcher
	{
    public:
        IOEventDispatcher(std::queue<IOEvent>& eventQ);
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

        std::queue<IOEvent>& refEventQ;
        std::unordered_map<IOEventType, bool>& refKeyStates;
        std::unordered_map<IOEventType, bool>& refMouseButtonStates;
        Vector2D<int>& refMousePosition;
        Vector2D<int>& refScrollDirection;
	};
}