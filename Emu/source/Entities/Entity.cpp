#pragma once

#include "../../include/Entities/Entity.h"
#include "../../include/Scenes/SceneObject.h"

namespace Engine 
{
	Entity::Entity(const Math::Vector2D<float> position, const Math::Vector2D<float> size, int directionFacing)
		: SceneObject(DYNAMIC, false, position, size, directionFacing)
	{}
} 