#pragma once

#include "../include/ISDL/ISDL.h"
#include "../include/Init.h"
#include "../include/Logging/Logger.h"
#include "../include/IDGenerator.h"
#include "../include/ComponentManager/ComponentManager.h"
#include "../include/Transform.h"
#include "../include/Physics/PhysicsBody.h"
#include "../include/ComponentManager/Updatable.h"

namespace Engine
{
	// Define the static members
	size_t IDGenerator::maxID = 0;
	std::unordered_set<size_t> IDGenerator::usedIDs;

	Init::Init()
	{
		IDGenerator::initialize(1000);

		ComponentManagerRegistry::GetManager<Updatable>().Allocate(5000);
		ComponentManagerRegistry::GetManager<PhysicsBody>().Allocate(5000);
		ComponentManagerRegistry::GetManager<Transform>().Allocate(5000);


		ENGINE_TRACE_D("Logger initialized!");

		if (!ISDL::ImgInit(IMG_INIT_PNG))
			ENGINE_CRITICAL("IMG Init failed! SDL_Error: " + std::string(ISDL::GetError()));

		if (ISDL::Init() < 0)
			ENGINE_CRITICAL("SDL Init failed! SDL_Error: " + std::string(ISDL::GetError()));
	}
}
