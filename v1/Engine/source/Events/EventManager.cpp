#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "../../include/Events/EventManager.h"

namespace Engine
{
	void EventManager::propogateEventQueue(SDL_Event& e)
	{
		
	}

	bool EventManager::pollEvents(SDL_Event& e)
	{
		return SDL_PollEvent(&e) != 0;
	}
}