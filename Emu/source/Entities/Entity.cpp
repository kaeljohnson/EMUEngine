#pragma once

#include <memory>

#include "../../include/Entities/Entity.h"
#include "../../include/Scenes/SceneObject.h"

namespace Engine 
{
	Entity::Entity(const Vector2D<float> position, const Vector2D<float> size, const std::shared_ptr<ITexture> ptrTexture, int directionFacing)
		: SceneObject(DYNAMIC, false, position, size, ptrTexture, directionFacing)
	{}
} 