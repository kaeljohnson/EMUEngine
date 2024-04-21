#pragma once

#include "box2d/box2d.h"

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

	World::~World()
	{
		ENGINE_INFO_D("Freeing World!");
		// Destroy all bodies in the world
		b2Body* body = m_world.GetBodyList();
		while (body != nullptr)
		{
			b2Body* nextBody = body->GetNext();
			m_world.DestroyBody(body);
			body = nextBody;
		}
	}

	void World::update()
	{
		m_world.Step(m_deltaTime, m_velocityIterations, m_positionIterations);
	}

	void World::addBox(Box* box)
	{
		if (box == nullptr)
		{
			ENGINE_ERROR_D("Box is null!");
			return;
		}

		box->m_body = m_world.CreateBody(&box->m_bodyDef);
		box->createFixture();

		ENGINE_INFO_D("Box added to world at position: {0}, {1}. With width: {2}, height: {3}",
			box->getCenterXInMeters(), box->getCenterYInMeters(), box->getWidthInMeters(), box->getHeightInMeters());
	}

	void World::removeBox(Box* body)
	{
		if (body == nullptr)
		{
			ENGINE_ERROR_D("Box is null!");
			return;
		}

		m_world.DestroyBody(body->m_body);
		body = nullptr;
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
}