#pragma once

#include "../include/Core.h"
#include "../include/Application.h"
#include "../include/Init.h"
#include "../include/Events/Event.h"
#include "../include/CallbackSystem/CallbackSystem.h"
#include "../include/Logging/Logger.h"
#include "../include/Scenes/SceneFactory.h"
#include "../include/Scenes/Scene.h"
#include "../include/Scenes/SceneManager.h"
#include "../include/Physics/BodyTypes.h"
#include "../include/Physics/Physics.h"
#include "../include/Components/PhysicsBody.h"
#include "../include/EventListeners/ContactEventListener.h"
#include "../include/Components/SimpleContact.h"
#include "../include/Tiles/TileMap.h"

#include "../include/Events/KeyStates.h"
#include "../include/Events/MouseStates.h"

#include "../include/Rendering/Screen.h"
#include "../include/Time.h"

#include "../include/Camera/Camera.h"

#include "../include/ECS/ECS.h"
#include "../include/MathUtil.h"

#include "../include/Components/Transform.h"
#include "../include/Components/Updatable.h"
#include "../include/ECS/ComponentManager.h"

namespace Engine
{
	using ScenePtr = std::shared_ptr<Scene>;
	using PhysicsBodyPtr = std::shared_ptr<PhysicsBody>;
	using ApplicationPtr = Application*;
	using ICallbackSystemPtr = ICallbackSystem*;
	using EntityID = size_t;
}