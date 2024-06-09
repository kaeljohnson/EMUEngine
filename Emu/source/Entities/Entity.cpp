#pragma once

#include <memory>

#include "../../include/Entities/Entity.h"
#include "../../include/Scenes/SceneObject.h"

namespace Engine 
{
	Entity::Entity(const float x, const float y, const float width, const float height, const std::shared_ptr<ITexture> ptrTexture)
		: SceneObject(DYNAMIC, false, x, y, width, height, ptrTexture)
	{}
}