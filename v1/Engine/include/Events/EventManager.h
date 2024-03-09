#pragma once

#include <SDL.h>
#include <queue>
#include <functional>
#include <variant>
#include <unordered_map>
#include <string>

#include "../../include/Actions/ActionTypes.h"
#include "../Events/Event.h"

namespace Engine
{
	// EventManager class. Polls for events and dispatches them to the event queue.
	class EventManager
	{
	private:
		using EventData = std::variant<int, float, std::string, std::pair<int, int>>;
		using EventCallback = std::function<void()>;

		std::queue<Event>& refEventQ;
		std::unordered_map<ActionType, EventCallback> m_eventCallbacks;

	public:
		//using EventCallback = std::function<void()>;

		EventManager(std::queue<Event>& eventQ);
		~EventManager() = default;

		void newEventCallback(const ActionType eventType, EventCallback callback);
		void triggerEventCallback(const ActionType eventType);

		// event handling and dispatching functions.
		void handleEvents();
		void dispatchQuitEvent();
		void dispatchWindowEvent(SDL_WindowEvent& windowEvent);
		void dispatchKeydownEvent(SDL_Keycode& keyCode);
		void dispatchKeyupEvent(SDL_Keycode& keyCode);
		void dispatchMouseMoveEvent(SDL_MouseMotionEvent& mouseMotion);
		void dispatchMouseButtonDownEvent(SDL_MouseButtonEvent& mouseButtonEvent);
		void dispatchMouseButtonUpEvent(SDL_MouseButtonEvent& mouseButtonEvent);
		void dispatchMouseScrollEvent(SDL_MouseWheelEvent& mouseWheelEvent);

		EventManager(const EventManager&) = delete;											
		EventManager& operator=(const EventManager&) = delete;
		EventManager(EventManager&&) = delete;
		EventManager& operator=(EventManager&&) = delete;
	};
}