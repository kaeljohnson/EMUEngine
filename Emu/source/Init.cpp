#pragma once

#include "../include/SDLWrapper/SDLWrapper.h"
#include "../include/Init.h"
#include "../include/Logging/Logger.h"

namespace Engine
{
	Init::Init()
	{

		ENGINE_TRACE_D("Logger initialized!");

		if (!ISDL::ImgInit(IMG_INIT_PNG))
			ENGINE_CRITICAL("IMG Init failed! SDL_Error: " + std::string(ISDL::GetError()));

		if (ISDL::Init() < 0)
			ENGINE_CRITICAL("SDL Init failed! SDL_Error: " + std::string(ISDL::GetError()));
	}
}
