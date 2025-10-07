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

		if (ISDL::ImgInit(IMG_INIT_PNG) < 0)
			ENGINE_CRITICAL("IMG Init failed! SDL_Error: " + std::string(ISDL::GetError()));

		if (ISDL::AudioInit(SDL_INIT_AUDIO) < 0)
			ENGINE_CRITICAL("SDL_mixer Init failed! Mix_Error: " + std::string(Mix_GetError()));

		if (ISDL::MixOpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
			ENGINE_CRITICAL("SDL_mixer OpenAudio failed! Mix_Error: " + std::string(Mix_GetError()));

		if (ISDL::Init() < 0)
			ENGINE_CRITICAL("SDL Init failed! SDL_Error: " + std::string(ISDL::GetError()));

		ENGINE_INFO_D("Initializing EMU with " + std::to_string(numEntitiesNeeded) + " entities.");
		EMU::Init(numEntitiesNeeded);
	}
}
