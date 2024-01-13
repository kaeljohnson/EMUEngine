#pragma once

#include <SDL.h>
#include <SDL_image.h>

namespace Engine
{
	class EventManager
	{
	private:

	public:
		void propogateEventQueue(SDL_Event& e);
		bool pollEvents(SDL_Event& e);


	};
}