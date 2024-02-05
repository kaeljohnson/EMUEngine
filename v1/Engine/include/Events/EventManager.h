#pragma once

#include <SDL.h>
#include <queue>

#include "EventHandlers.h"
#include "../Actions/ActionsEnum.h"


namespace Engine
{
	class EventManager
	{
	private:
		EventHandlers& refEventHandlers;

		std::queue<Actions>& refActionsQ;

	public:
		EventManager(EventHandlers& eventHandlers, std::queue<Actions>& actionsQ);

		void handleEvents();
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