#pragma once

#include "../include/SDLWrapper/SDLWrapper.h"
#include "../include/Init.h"
#include "../include/Logging/Logger.h"

namespace Engine
{
	Init::Init()
	{
		Logger::Init();
		ENGINE_TRACE_D("Logger initialized!");

		if (!IMG_INIT(IMG_INIT_PNG))
			ENGINE_CRITICAL("IMG Init failed! SDL_Error: {}", SDL_GET_ERROR());

		if (SDL_INIT() < 0)
			ENGINE_CRITICAL("SDL Init failed! SDL_Error: {}", SDL_GET_ERROR());
	}
}
