#pragma once

#include <queue>

#include "../../include/Events/Event.h"
#include "../../include/GameObjects/GameObject.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	GameObject::GameObject(int startingX, int startingY, bool visible, bool collidable)
		: m_x(startingX), m_y(startingY), m_visible(visible), m_collidable(collidable), 
		  m_rect({ startingX, startingY, 80, 80 }), m_texture(nullptr)
	{
	}

	void GameObject::processEvents(const std::queue<Event>& eventQ)
	{
		if (eventQ.size() > 0) ENGINE_INFO("GameObject::processEvents() called {}", static_cast<int>(eventQ.front().m_eventType));
	}
}