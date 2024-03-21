#pragma once

#include "../../include/Physics/World.h"
#include "../../include/Physics/Box.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	World::World(const float gravityX, const float gravityY, const float deltaTime, const int velocityIterations, const int positionIterations)
		: m_gravity({ gravityX, gravityY }), m_world({ gravityX * PIXELS_PER_METER, gravityY * PIXELS_PER_METER}), m_deltaTime(deltaTime),
		m_velocityIterations(velocityIterations), m_positionIterations(positionIterations)
	{}

	void World::update()
	{
		m_world.Step(m_deltaTime, m_velocityIterations, m_positionIterations);
	}

	void World::addBox(Box& box)
	{
		box.setBody( m_world.CreateBody(&box.getBodyDef()) );
		box.createFixture();

		ENGINE_INFO("Box added to world at position: {0}, {1}. With width: {2}, height: {3}",
			box.getCenterXInMeters(), box.getCenterYInMeters(), box.getWidthInMeters(), box.getHeightInMeters());
	}
}