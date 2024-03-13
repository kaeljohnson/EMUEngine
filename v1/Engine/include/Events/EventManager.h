#pragma once

#include <SDL.h>
#include <queue>

#include "../Events/Event.h"

namespace Engine
{
	// EventManager class. Polls for events and dispatches them to the event queue.
	class EventManager
	{
	private:
		std::queue<Event>& refEventQ;

		void dispatchQuitEvent();
		void dispatchWindowEvent(SDL_WindowEvent& windowEvent);
		void dispatchKeydownEvent(SDL_Keycode& keyCode);
		void dispatchKeyupEvent(SDL_Keycode& keyCode);
		void dispatchMouseMoveEvent(SDL_MouseMotionEvent& mouseMotion);
		void dispatchMouseButtonDownEvent(SDL_MouseButtonEvent& mouseButtonEvent);
		void dispatchMouseButtonUpEvent(SDL_MouseButtonEvent& mouseButtonEvent);
		void dispatchMouseScrollEvent(SDL_MouseWheelEvent& mouseWheelEvent);
	public:
		EventManager(std::queue<Event>& eventQ);
		~EventManager() = default;

		// event handling and dispatching functions.
		void handleEvents();

		EventManager(const EventManager&) = delete;											
		EventManager& operator=(const EventManager&) = delete;
		EventManager(EventManager&&) = delete;
		EventManager& operator=(EventManager&&) = delete;
	};
}