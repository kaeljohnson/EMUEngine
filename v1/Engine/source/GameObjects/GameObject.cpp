#pragma once

#include "../../include/GameObjects/GameObject.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Physics/ConversionFunctions.h"
#include "../../include/Physics/Box.h"
#include "../../include/Physics/BodyTypes.h"
#include "../../include/Textures/Texture.h"

namespace Engine
{
	GameObject::GameObject(const BodyType bodyType, const float startingX, const float startingY, const float width, const float height, 
		const float density, const float friction,
		const bool visible, const bool fixed, const bool collidable, Texture* texture)
		: Box(bodyType, startingX, startingY, width / 2.0f, height / 2.0f,  density, friction),
		prevX(startingX), prevY(startingY), X(startingX), Y(startingY), m_width(width), m_height(height), m_density(density), m_friction(friction),
		m_visible(visible), m_fixed(fixed), m_collidable(collidable),
		m_texture(texture)
	{
		ENGINE_INFO("GameObject created at: ({0}, {1})", startingX, startingY);
	}

	Texture* GameObject::GetTexture()
	{
		return m_texture;
	}
}