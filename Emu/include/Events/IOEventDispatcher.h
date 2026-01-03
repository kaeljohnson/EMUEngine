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

    /**
	* @brief Dispatches SDL2 events to the IOEventQueue.
    * 
	* This class listens for SDL2 events and translates them into custom IOEvent types,
    */
	class IOEventDispatcher
	{
    public:
        IOEventDispatcher();

        /**
		* @brief Polls for SDL2 events and dispatches them to the event queue.
        * 
		* This function should be called every frame.
        */
        void PollEvents();

        /**
		* @brief Sets up the eventQ.
        */
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

		IOEventQueue* ptrEventQ;                                        /// Pointer to the event queue to dispatch events to.
		std::unordered_map<IOEventType, bool>& refKeyStates;            /// Reference to the key states map.
		std::unordered_map<IOEventType, bool>& refMouseButtonStates;	/// Reference to the mouse button states map.
		Math2D::Point2D<int>& refMousePosition;							/// Reference to the mouse position.
		Math2D::Point2D<int>& refScrollDirection;						/// Reference to the mouse scroll direction.
	};
}