#pragma once

#include "../../include/GameObjects/GameObject.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	GameObject::GameObject(const int startingX, const int startingY, bool visible, bool collidable)
		: m_x(startingX), m_y(startingY), m_visible(visible), m_collidable(collidable), 
		  m_rect({ startingX, startingY, 80, 80 }), m_texture(nullptr)
	{
		ENGINE_INFO("GameObject created at: ({0}, {1})", m_x, m_y);
	}
}