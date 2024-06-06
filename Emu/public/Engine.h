#pragma once

#include "../include/Core.h"
#include "../include/Application.h"
#include "../include/Init.h"
#include "../include/Events/Event.h"
#include "../include/CallbackSystem/CallbackSystem.h"
#include "../include/Logging/Logger.h"
#include "../include/Scenes/SceneFactory.h"
#include "../include/Scenes/SceneObject.h"
#include "../include/Scenes/Scene.h"
#include "../include/Events/EventListener.h"
#include "../include/Physics/BodyTypes.h"
#include "../include/Textures/ITexture.h"
#include "../include/Physics/IPhysicsBody.h"
#include "../include/Physics/PhysicsFactory.h"
#include "../include/Controllable/Controllable.h"

namespace Engine
{
	using ScenePtr = std::shared_ptr<Scene>;
	using SceneObjectPtr = std::shared_ptr<SceneObject>;
	using PhysicsBodyPtr = std::shared_ptr<IPhysicsBody>;
	using ApplicationPtr = Application*;
	using ICallbackSystemPtr = ICallbackSystem*;
}