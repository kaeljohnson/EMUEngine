#pragma once

#include <box2d/box2d.h>
#include "../../include/Physics/Physics.h"
#include "../../include/ECS/ECS.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Physics/ContactSystem.h"
#include "../../include/Components.h"
#include "../../include/Time.h"
#include "../../include/MathUtil.h"

namespace Engine
{
	// Physics Interface functions

	PhysicsInterface::PhysicsInterface(ECS& refECS) : m_refECS(refECS)
	{
		ENGINE_INFO_D("Physics Interface created!");
	}

	void PhysicsInterface::CreateBody(Entity* ptrEntity)
	{
		m_refECS.AddComponent<PhysicsBody>(ptrEntity);
	}

	void PhysicsInterface::SetBodyType(Entity* ptrEntity, const BodyType type)
	{
		GetBody(ptrEntity)->m_bodyType = type;
	}

	void PhysicsInterface::SetDimensions(Entity* ptrEntity, const Vector2D<float> dimensions)
	{
		PhysicsBody* ptrBody = GetBody(ptrEntity);
		ptrBody->m_dimensions = dimensions;
		ptrBody->m_halfDimensions = dimensions / 2.0f;
	}

	PhysicsBody* PhysicsInterface::GetBody(Entity* ptrEntity)
	{
		return m_refECS.GetComponent<PhysicsBody>(ptrEntity);
	}

	void PhysicsInterface::SetStartingPosition(Entity* ptrEntity, Vector2D<float> position)
	{
		PhysicsBody* ptrBody = GetBody(ptrEntity);
		ptrBody->m_startingPosition = position;
	}

	void PhysicsInterface::SetPosition(Entity* ptrEntity, Vector2D<float> position)
	{
		b2BodyId bodyId = *GetBody(ptrEntity)->m_bodyId;
		b2Rot rotation = b2Body_GetRotation(bodyId);
		b2Body_SetTransform(bodyId, b2Vec2(position.X, position.Y), rotation);
	}

	const Vector2D<float> PhysicsInterface::GetPosition(Entity* ptrEntity)
	{
		b2BodyId bodyId = *GetBody(ptrEntity)->m_bodyId;
		b2Vec2 position = b2Body_GetPosition(bodyId);
		return Vector2D<float>(position.x, position.y);
	}

	const Vector2D<float> PhysicsInterface::GetTopLeftPosition(Entity* ptrEntity)
	{
		PhysicsBody* ptrBody = GetBody(ptrEntity);
		b2Vec2 position = b2Body_GetPosition(*ptrBody->m_bodyId);
		return Vector2D<float>(position.x - ptrBody->m_halfDimensions.X, position.y - ptrBody->m_halfDimensions.Y);
	}

	void PhysicsInterface::ApplyForceToBody(Entity* ptrEntity, Vector2D<float> force)
	{
		b2BodyId bodyId = *GetBody(ptrEntity)->m_bodyId;
		b2Body_ApplyForceToCenter(bodyId, b2Vec2(force.X, force.Y), true);
	}

	void PhysicsInterface::ApplyImpulseToBody(Entity* ptrEntity, Vector2D<float> impulse)
	{
		b2BodyId bodyId = *GetBody(ptrEntity)->m_bodyId;
		b2Body_ApplyLinearImpulseToCenter(bodyId, b2Vec2(impulse.X, impulse.Y), true);
	}

	// PhysicsBody2d getter and setter wrappers
	void PhysicsInterface::SetGravity(Entity* ptrEntity, bool enabled)
	{
		PhysicsBody* ptrBody = GetBody(ptrEntity);
		ptrBody->m_gravityOn = enabled;
		b2BodyId bodyId = *ptrBody->m_bodyId;
		b2Body_SetGravityScale(bodyId, enabled ? 1.0f : 0.0f);

	}

	void PhysicsInterface::SetDeceleration(Entity* ptrEntity, const float decel)
	{
		b2BodyId bodyId = *GetBody(ptrEntity)->m_bodyId;
		b2Body_SetLinearDamping(bodyId, decel);

	}

	void PhysicsInterface::SetVelocity(Entity* ptrEntity, const Vector2D<float> velocity)
	{
		b2BodyId bodyId = *GetBody(ptrEntity)->m_bodyId;
		b2Body_SetLinearVelocity(bodyId, b2Vec2(velocity.X, velocity.Y));
	}

	void PhysicsInterface::SetXVelocity(Entity* ptrEntity, const float xVel)
	{
		b2BodyId bodyId = *GetBody(ptrEntity)->m_bodyId;
		b2Vec2 velocity = b2Body_GetLinearVelocity(bodyId);
		b2Body_SetLinearVelocity(bodyId, b2Vec2(xVel, velocity.y));
	}

	void PhysicsInterface::SetYVelocity(Entity* ptrEntity, const float yVel)
	{
		b2BodyId bodyId = *GetBody(ptrEntity)->m_bodyId;
		b2Vec2 velocity = b2Body_GetLinearVelocity(bodyId);
		b2Body_SetLinearVelocity(bodyId, b2Vec2(velocity.x, yVel));
	}

	const Vector2D<float> PhysicsInterface::GetVelocity(Entity* ptrEntity)
	{
		b2BodyId bodyId = *GetBody(ptrEntity)->m_bodyId;
		b2Vec2 velocity = b2Body_GetLinearVelocity(bodyId);
		return Vector2D<float>(velocity.x, velocity.y);
	}

	void PhysicsInterface::SetRestitution(Entity* ptrEntity, const float restitution)
	{
		b2ShapeId shapeId = *GetBody(ptrEntity)->m_shapeId;
		b2Shape_SetRestitution(shapeId, restitution);
	}

	void PhysicsInterface::SetDensity(Entity* ptrEntity, const float density)
	{
		b2ShapeId shapeId = *GetBody(ptrEntity)->m_shapeId;
		b2Shape_SetDensity(shapeId, density, true);
	}

	void PhysicsInterface::SetFriction(Entity* ptrEntity, const float friction)
	{
		b2ShapeId shapeId = *GetBody(ptrEntity)->m_shapeId;
		b2Shape_SetFriction(shapeId, friction);
	}

	void PhysicsInterface::RemoveBodyFromWorld(Entity* ptrEntity)
	{
		PhysicsBody* ptrBody = GetBody(ptrEntity);
		if (ptrBody->m_bodyId == nullptr) return;

		b2DestroyBody(*ptrBody->m_bodyId);
		ptrBody->m_bodyId = nullptr;
		ptrBody->m_shapeId = nullptr;
		ptrBody->m_worldId = nullptr;
	}

	void PhysicsInterface::SetFixedRotation(Entity* ptrEntity, bool fixed)
	{
		b2BodyId bodyId = *GetBody(ptrEntity)->m_bodyId;
		b2Body_SetFixedRotation(bodyId, fixed);
	}

	const float PhysicsInterface::GetAngleInRadians(Entity* ptrEntity)
	{
		b2BodyId bodyId = *GetBody(ptrEntity)->m_bodyId;
		b2Rot rotation = b2Body_GetRotation(bodyId);
		float angleInRadians = b2Rot_GetAngle(rotation);
		return angleInRadians;
	}

	const float PhysicsInterface::GetAngleInDegrees(Entity* ptrEntity)
	{
		b2BodyId bodyId = *GetBody(ptrEntity)->m_bodyId;
		b2Rot rotation = b2Body_GetRotation(bodyId);
		float angleInRadians = b2Rot_GetAngle(rotation);
		return angleInRadians * 180.0f / 3.14159265359f;

	}

	// Physics Simulation
	PhysicsSimulation::PhysicsSimulation(ECS& refECS, const Vector2D<float> gravity)
		: m_refECS(refECS), m_ptrWorldId(nullptr), m_contactSystem(refECS)
	{
		ENGINE_INFO_D("Creating World!");

		b2WorldDef worldDef = b2DefaultWorldDef();
		worldDef.gravity = { gravity.X, gravity.Y };
		m_ptrWorldId = new b2WorldId(b2CreateWorld(&worldDef));

		ENGINE_INFO_D("World created!");
	}

	void PhysicsSimulation::UpdateGravity(const Vector2D<float> gravity)
	{
		b2WorldDef worldDef = b2DefaultWorldDef();
		worldDef.gravity = { gravity.X, gravity.Y };
		b2World_SetGravity(*m_ptrWorldId, worldDef.gravity);
	}

	void PhysicsSimulation::AddPhysicsBodiesToWorld()
	{
		ComponentManager<PhysicsBody>& physicsBodyManager = m_refECS.GetComponentManager<PhysicsBody>();
		for (PhysicsBody& refPhysicsBody : physicsBodyManager)
		{
			if (!refPhysicsBody.IsActive()) continue;

			b2BodyDef bodyDef = b2DefaultBodyDef();
			b2ShapeDef shapeDef = b2DefaultShapeDef();

			switch (refPhysicsBody.m_bodyType)
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
				shapeDef.isSensor = true;
				break;
			default:
				bodyDef.type = b2_staticBody;
				break;
			}

			bodyDef.position =
			{
				refPhysicsBody.m_startingPosition.X + refPhysicsBody.m_halfDimensions.X,
				refPhysicsBody.m_startingPosition.Y + refPhysicsBody.m_halfDimensions.Y
			};

			bodyDef.fixedRotation = true;
			bodyDef.userData = refPhysicsBody.GetEntity(); // NOTE: This is the entity of the physics body. Not a pointer to the physics body.

			b2BodyId bodyId = b2CreateBody(*m_ptrWorldId, &bodyDef);

			shapeDef.density = 1.0f;
			shapeDef.friction = 0.0f;
			shapeDef.restitution = 0.0f;

			b2Polygon box = b2MakeBox(refPhysicsBody.m_halfDimensions.X, refPhysicsBody.m_halfDimensions.Y);
			b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box);

			refPhysicsBody.m_bodyId = new b2BodyId(bodyId);
			refPhysicsBody.m_shapeId = new b2ShapeId(shapeId);
			refPhysicsBody.m_worldId = m_ptrWorldId;
		}
	}

	void PhysicsSimulation::Update()
	{
		b2World_Step(*m_ptrWorldId, Time::GetTimeStep(), 4);
		m_contactSystem.ProcessContacts(m_ptrWorldId);
	}

	void PhysicsSimulation::DestroyWorld()
	{
		if (m_ptrWorldId == nullptr)
		{
			ENGINE_INFO_D("World is already null. No need to destroy.");
			return;
		}

		ENGINE_INFO_D("Freeing World!");

		b2DestroyWorld(*m_ptrWorldId);

		for (PhysicsBody& refPhysicsBody : m_refECS.GetComponentManager<PhysicsBody>())
		{
			refPhysicsBody.m_bodyId = nullptr;
			refPhysicsBody.m_shapeId = nullptr;
			refPhysicsBody.m_worldId = nullptr;
		}

		delete m_ptrWorldId;
		m_ptrWorldId = nullptr;
		ENGINE_INFO_D("World freed!");
	}

	void PhysicsSimulation::Cleanup()
	{
		if (m_ptrWorldId != nullptr)
		{
			DestroyWorld();
		}
	}

	// Temp
	void PhysicsBody::OnDeactivate()
	{
		if (GameState::IN_SCENE)
			b2Body_SetAwake(*m_bodyId, false);
	}

	void PhysicsBody::OnActivate()
	{
		if (GameState::IN_SCENE)
			b2Body_SetAwake(*m_bodyId, true);
	}
}