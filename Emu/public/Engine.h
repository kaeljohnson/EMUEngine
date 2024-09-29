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
#include "../include/Physics/PhysicsBody.h"
#include "../include/Tiles/Tile.h"
#include "../include/Tiles/TileMap.h"

#include "../include/Rendering/Screen.h"

#include "../include/Camera/Camera.h"
#include "../include/Camera/ScrollingCamera.h"
#include "../include/Camera/TargetCamera.h"

#include "../include/CommonFunctions.h"
#include "../include/MathUtil.h"

#include "../include/Transform.h"
#include "../include/ComponentManager/Updatable.h"

namespace Engine
{
	using ScenePtr = std::shared_ptr<Scene>;
	using PhysicsBodyPtr = std::shared_ptr<PhysicsBody>;
	using ApplicationPtr = Application*;
	using ICallbackSystemPtr = ICallbackSystem*;
}