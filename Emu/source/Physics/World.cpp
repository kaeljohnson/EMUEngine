#pragma once

#include "box2d/box2d.h"

#include "../../include/EngineConstants.h"
#include "../../include/Physics/World.h"
#include "../../include/Physics/PhysicsBody.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	World::World() : m_world(new b2World(b2Vec2(0.0f, 0.0f))), m_gravity(Vector2D( 0.0f, 0.0f )), m_velocityIterations(0), m_positionIterations(0) {}

	World::World(const float gravityX, const float gravityY, const int velocityIterations, const int positionIterations)
		: m_gravity({ gravityX, gravityY }), m_world(new b2World(b2Vec2( gravityX, gravityY))),
		m_velocityIterations(velocityIterations), m_positionIterations(positionIterations)
	{}

	World::~World()
	{
		ENGINE_INFO_D("Freeing World!");
		// Destroy all bodies in the world
		b2Body* body = m_world->GetBodyList();
		while (body != nullptr)
		{
			b2Body* nextBody = body->GetNext();
			m_world->DestroyBody(body);
			body = nextBody;
		}

		delete m_world;
	}

	void World::Update()
	{
		m_world->Step(TIME_STEP, m_velocityIterations, m_positionIterations);
	}

	void World::AddBody(std::shared_ptr<PhysicsBody> physicsBody)
	{
		if (physicsBody == nullptr)
		{
			ENGINE_ERROR_D("PhysicsBody is null!");
			return;
		}

		b2Body* body;
		b2BodyDef bodyDef;
		b2FixtureDef fixtureDef;
		b2PolygonShape shape;

		switch (physicsBody->GetBodyType())
		{
		case STATIC:
			bodyDef.type = b2_staticBody;
			break;
		case DYNAMIC:
			bodyDef.type = b2_dynamicBody;
			break;
		case KINEMATIC:
			bodyDef.type = b2_kinematicBody;
			break;
		case SENSOR:
			bodyDef.type = b2_kinematicBody;
			physicsBody->SetIsSensor(true);
			break;
		default:
			bodyDef.type = b2_staticBody;
			break;
		}

		bodyDef.fixedRotation = physicsBody->GetIsRotationFixed();
		bodyDef.userData.pointer = reinterpret_cast<intptr_t>(physicsBody.get());
		bodyDef.position.Set(physicsBody->GetStartingPosition().X + physicsBody->GetHalfWidth(), physicsBody->GetStartingPosition().Y + physicsBody->GetHalfHeight());

		body = m_world->CreateBody(&bodyDef);
		
		shape.SetAsBox(physicsBody->GetHalfWidth(), physicsBody->GetHalfHeight());
		fixtureDef.shape = &shape;
		fixtureDef.restitution = physicsBody->GetStartingRestitution();
		fixtureDef.restitutionThreshold = physicsBody->GetStartingRestitutionThreshold();
		fixtureDef.density = physicsBody->GetStartingDensity();
		fixtureDef.friction = physicsBody->GetStartingFriction();
		fixtureDef.isSensor = physicsBody->GetIsSensor();
		body->CreateFixture(&fixtureDef);

		physicsBody->m_body = body;
	}

	void World::RemoveBody(std::shared_ptr<PhysicsBody> physicsBody)
	{
		if (physicsBody == nullptr)
		{
			ENGINE_ERROR_D("physicsBody is null!");
			return;
		}
		physicsBody->RemoveBodyFromWorld();
	}

	void World::SetGravity(const float gravityX, const float gravityY)
	{
		m_gravity = { gravityX, gravityY };
		m_world->SetGravity({ gravityX, gravityY });
	}
}