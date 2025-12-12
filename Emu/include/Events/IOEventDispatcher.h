#pragma once

#include "../Includes.h"
#include "../ISDL/ISDL.h"
#include "../MathUtil.h"
#include "IOEvent.h"
#include "KeyStates.h"
#include "MouseStates.h"

namespace Engine
{
    using IOEventQueue = std::queue<IOEvent>;

	class IOEventDispatcher
	{
    public:
        IOEventDispatcher();
        void PollEvents();
		void SetEventQueue(IOEventQueue* eventQ);
    private:
        void dispatchQuitEvent();   
        void dispatchWindowEvent(SDL_WindowEvent& windowEvent);
        void dispatchKeydownEvent(SDL_Keycode& keyCode);
        void dispatchKeyupEvent(SDL_Keycode& keyCode);
        void dispatchMouseMoveEvent(SDL_MouseMotionEvent& mouseMotion);
        void dispatchMouseButtonDownEvent(SDL_MouseButtonEvent& mouseButtonEvent);
        void dispatchMouseButtonUpEvent(SDL_MouseButtonEvent& mouseButtonEvent);
        void dispatchMouseScrollEvent(SDL_MouseWheelEvent& mouseWheelEvent);

        IOEventQueue* ptrEventQ;
        std::unordered_map<IOEventType, bool>& refKeyStates;
        std::unordered_map<IOEventType, bool>& refMouseButtonStates;
        Math2D::Point2D<int>& refMousePosition;
        Math2D::Point2D<int>& refScrollDirection;
	};
}