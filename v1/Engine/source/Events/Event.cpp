#pragma once

#include "../../include/Events/Event.h"

namespace Engine
{
	Event::Event(SDL_Event& e)
		: m_eventHandled(false),
		  m_event(e)
	{}

}