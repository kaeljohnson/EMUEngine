#pragma once

#include <SDL.h>

#include "EventEnums.h"

namespace Engine
{
	struct Event
	{
		SDL_Event m_event;
		bool m_eventHandled;

		Event(SDL_Event& e);
	};
}