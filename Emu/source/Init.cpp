#pragma once

#include "../include/ISDL/ISDL.h"
#include "../Public/Logger.h"
#include "../Public/EMU.h"
#include "../public/Init.h"

namespace Engine
{
    Init::Init(const size_t numEntitiesNeeded)
    {
        ENGINE_LOG_D("Logger initialized!");

        // 1. Initialize SDL first
        if (ISDL::Init() < 0)
        {
            ENGINE_ERROR("SDL Init failed! SDL_Error: {}", ISDL::GetError());
            std::exit(1);
        }

        // 2. Initialize SDL_image
        if (ISDL::ImgInit(IMG_INIT_PNG) < 0)
        {
            ENGINE_ERROR("IMG Init failed! SDL_Error: {}", ISDL::GetError());
            std::exit(1);
        }

        // 3. Initialize SDL audio subsystem
        if (ISDL::AudioInit(SDL_INIT_AUDIO) < 0)
        {
            ENGINE_ERROR("SDL Audio Init failed! SDL_Error: {}", ISDL::GetError());
            std::exit(1);
        }

        // 4. Open SDL_mixer audio
        if (ISDL::MixOpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            ENGINE_ERROR("SDL_mixer OpenAudio failed! Mix_Error: {}", Mix_GetError());
            std::exit(1);
        }

        // Allocate 32 channels for audio. 
        // Should be sufficient for most games. If running out of 
		// channels, consider increasing implementing a channel manager.
		Mix_AllocateChannels(32); 

        ENGINE_INFO_D("Initializing EMU with {} entities.", numEntitiesNeeded);
        EMU::Init(numEntitiesNeeded);
    }

}
