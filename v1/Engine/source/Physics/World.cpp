#pragma once

#include "../../include/Physics/World.h"
#include "../../include/Physics/Box.h"

namespace Engine
{
	World::World(float gravityX, float gravityY, float deltaTime, int velocityIterations, int positionIterations)
		: m_gravity({ gravityX, gravityY }), m_world({ gravityX, gravityY }), m_deltaTime(deltaTime),
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
	}
}