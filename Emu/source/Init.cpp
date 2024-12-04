#pragma once

#include "../include/ISDL/ISDL.h"
#include "../include/Init.h"
#include "../include/Logging/Logger.h"
#include "../include/ECS/ECS.h"
#include "../include/ECS/ComponentManager.h"
#include "../include/Components/Transform.h"
#include "../include/Components/PhysicsBody.h"
#include "../include/Components/Updatable.h"
#include "../include/Components/SimpleContact.h"
#include "../include/EventListeners/EventListener.h"
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
		ECS::RegisterComponentManager<SimpleContact>();

		// Event listeners will be components for now.
		ECS::RegisterComponentManager<ContactEventListener>();
		ECS::RegisterComponentManager<SensorEventListener>();

		if (!ISDL::ImgInit(IMG_INIT_PNG))
			ENGINE_CRITICAL("IMG Init failed! SDL_Error: " + std::string(ISDL::GetError()));

		if (ISDL::Init() < 0)
			ENGINE_CRITICAL("SDL Init failed! SDL_Error: " + std::string(ISDL::GetError()));
	}
}
