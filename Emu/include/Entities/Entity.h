#pragma once

#include <memory>

#include "../Core.h"

#include "../Scenes/SceneObject.h"
#include "../Textures/ITexture.h"

namespace Engine
{
	class Entity : public SceneObject
	{
	public:
		EMU_API Entity(const float x, const float y, const float width, const float height, const std::shared_ptr<ITexture> ptrTexture);
		~Entity() = default;
	};
}