#pragma once

#include "../include/ISDL/ISDL.h"
#include "../include/Logging/Logger.h"
#include "../include/EMU.h"
#include "../include/Init.h"

namespace Engine
{
	Init::Init(const size_t numEntitiesNeeded)
	{
		ENGINE_TRACE_D("Logger initialized!");

		if (!ISDL::ImgInit(IMG_INIT_PNG))
			ENGINE_CRITICAL("IMG Init failed! SDL_Error: " + std::string(ISDL::GetError()));

		if (ISDL::Init() < 0)
			ENGINE_CRITICAL("SDL Init failed! SDL_Error: " + std::string(ISDL::GetError()));

		ENGINE_CRITICAL_D("Allocating " + std::to_string(numEntitiesNeeded) + " entities.");
		EMU::Init(numEntitiesNeeded);
	}
}
