#pragma once

#include <memory>

#include "../Core.h"
#include "../MathUtil.h"

#include "../Scenes/SceneObject.h"
#include "../Textures/ITexture.h"

namespace Engine
{
	class Entity : public SceneObject
	{
	public:
		EMU_API Entity(const Vector2D position, const Vector2D size, const std::shared_ptr<ITexture> ptrTexture, int directionFacing);
		~Entity() = default;
	};
}