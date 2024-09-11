#pragma once

#include <memory>

#include "../../include/Entities/Entity.h"
#include "../../include/Scenes/SceneObject.h"

namespace Engine 
{
	Entity::Entity(const Vector2D<float> position, const Vector2D<float> size, int directionFacing, const bool visible)
		: SceneObject(DYNAMIC, false, position, size, directionFacing, visible)
	{}
} 