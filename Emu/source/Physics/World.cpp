#pragma once

#include "box2d/box2d.h"

#include "../../include/EngineConstants.h"
#include "../../include/Physics/World.h"
#include "../../include/Physics/Box.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	World::World() : m_world({ 0.0f, 0.0f }), m_gravity({ 0.0f, 0.0f }), m_velocityIterations(0), m_positionIterations(0) {}

	World::World(const float gravityX, const float gravityY, const int velocityIterations, const int positionIterations)
		: m_gravity({ gravityX, gravityY }), m_world({ gravityX, gravityY}),
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

	void World::Update()
	{
		m_world.Step(TIME_STEP, m_velocityIterations, m_positionIterations);
	}

	void World::AddBox(std::shared_ptr<Box> box)
	{
		if (box == nullptr)
		{
			ENGINE_ERROR_D("Box is null!");
			return;
		}

		box->m_body = m_world.CreateBody(&box->m_bodyDef);
		box->CreateFixture();

		ENGINE_INFO_D("Box added to world at position: " + std::to_string(box->GetCenterXInMeters()) + ", " 
			+ std::to_string(box->GetCenterYInMeters()) + ". With width: " 
			+ std::to_string(box->GetWidthInMeters()) + ", height: " + std::to_string(box->GetHeightInMeters()));
	}

	void World::RemoveBox(std::shared_ptr<Box> box)
	{
		if (box == nullptr)
		{
			ENGINE_ERROR_D("Box is null!");
			return;
		}
		box->RemoveBodyFromWorld();

		/*m_world.DestroyBody(box->m_body);
		box->m_body = nullptr;*/
	}

	void World::SetGravity(const float gravityX, const float gravityY)
	{
		m_gravity = { gravityX, gravityY };
		m_world.SetGravity({ gravityX, gravityY });
	}
}