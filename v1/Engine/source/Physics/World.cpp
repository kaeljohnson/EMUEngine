#pragma once

#include "../../include/Physics/World.h"
#include "../../include/Physics/Box.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	World::World() : m_world({ 0.0f, 0.0f }), m_gravity({ 0.0f, 0.0f }), m_deltaTime(0), m_velocityIterations(0), m_positionIterations(0) {}

	World::World(const float gravityX, const float gravityY, const float deltaTime, const int velocityIterations, const int positionIterations)
		: m_gravity({ gravityX, gravityY }), m_world({ gravityX, gravityY}), m_deltaTime(deltaTime),
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

	void World::removeBox(Box& body)
	{
		m_world.DestroyBody(body.getBody());
	}

	void World::SetGravity(const float gravityX, const float gravityY)
	{
		m_gravity = { gravityX, gravityY };
		m_world.SetGravity({ gravityX, gravityY });
	}

	void World::SetTimeStep(const float timeStep)
	{
		m_deltaTime = timeStep;
	}

	void World::SetVelocityIterations(const int velocityIterations)
	{
		m_velocityIterations = velocityIterations;
	}

	void World::SetPositionIterations(const int positionIterations)
	{
		m_positionIterations = positionIterations;
	}
}