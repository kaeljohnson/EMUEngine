#pragma once

#include "../include/ISDL/ISDL.h"
#include "../include/Init.h"
#include "../include/Logging/Logger.h"
#include "../include/ECS/ECS.h"
#include "../include/ECS/ComponentManager.h"
#include "../include/Transform.h"
#include "../include/Physics/PhysicsBody.h"
#include "../include/Updatable/Updatable.h"
#include "../include/Camera/Camera.h"

namespace Engine
{
	Init::Init()
	{
		ENGINE_TRACE_D("Logger initialized!");

		ECS::initialize(10000);

		ECS::RegisterComponentManager<Updatable>();
		ECS::RegisterComponentManager<PhysicsBody>();
		ECS::RegisterComponentManager<Transform>();
		ECS::RegisterComponentManager<Camera>();

		if (!ISDL::ImgInit(IMG_INIT_PNG))
			ENGINE_CRITICAL("IMG Init failed! SDL_Error: " + std::string(ISDL::GetError()));

		if (ISDL::Init() < 0)
			ENGINE_CRITICAL("SDL Init failed! SDL_Error: " + std::string(ISDL::GetError()));
	}
}
