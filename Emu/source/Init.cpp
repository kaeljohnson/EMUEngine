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

        // 1. Initialize SDL first
        if (ISDL::Init() < 0)
            ENGINE_CRITICAL("SDL Init failed! SDL_Error: {}", ISDL::GetError());

        // 2. Initialize SDL_image
        if (ISDL::ImgInit(IMG_INIT_PNG) < 0)
            ENGINE_CRITICAL("IMG Init failed! SDL_Error: {}", ISDL::GetError());

        // 3. Initialize SDL audio subsystem
        if (ISDL::AudioInit(SDL_INIT_AUDIO) < 0)
            ENGINE_CRITICAL("SDL Audio Init failed! SDL_Error: {}", ISDL::GetError());

        // 4. Open SDL_mixer audio
        if (ISDL::MixOpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
            ENGINE_CRITICAL("SDL_mixer OpenAudio failed! Mix_Error: {}", Mix_GetError());

        // Allocate 32 channels for audio. 
        // Should be sufficient for most games. If running out of 
		// channels, consider increasing implementing a channel manager.
		Mix_AllocateChannels(32); 

        ENGINE_INFO_D("Initializing EMU with {} entities.", numEntitiesNeeded);
        EMU::Init(numEntitiesNeeded);
    }

}
