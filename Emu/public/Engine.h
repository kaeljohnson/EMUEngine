#pragma once

#include "../include/Init.h"
#include "../include/Logging/Logger.h"
#include "../include/Events/KeyStates.h"
#include "../include/Rendering/Screen.h"
#include "../include/Time.h"

#include "../include/EMU.h"

namespace Engine
{
	using ScenePtr = std::shared_ptr<Scene>;
	using PhysicsBodyPtr = std::shared_ptr<PhysicsBody>;
	using EntityID = size_t;
}