#pragma once

#include "../include/ISDL/ISDL.h"
#include "../include/Init.h"
#include "../include/Logging/Logger.h"
#include "../include/ECS/ECS.h"
#include "../include/Events/IOEventSystem.h"
#include "../include/ECS/ComponentManager.h"
#include "../include/Components.h"
#include "../include/Camera/Camera.h"
#include "../include/EMU.h"

namespace Engine
{
	Init::Init()
	{
		ENGINE_TRACE_D("Logger initialized!");

		IOEventSystem::Initialize();

		if (!ISDL::ImgInit(IMG_INIT_PNG))
			ENGINE_CRITICAL("IMG Init failed! SDL_Error: " + std::string(ISDL::GetError()));

		if (ISDL::Init() < 0)
			ENGINE_CRITICAL("SDL Init failed! SDL_Error: " + std::string(ISDL::GetError()));

		EMU::Init();
	}
}
