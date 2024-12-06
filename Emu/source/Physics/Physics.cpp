#pragma once

#include <box2d/box2d.h>

#include "../../include/Physics/Physics.h"
#include "../../include/Components.h"
#include "../../include/EventListeners.h"
#include "../../include/ECS/ECS.h"
#include "../../include/Time.h"
#include "../../include/Logging/Logger.h"

#include "../../include/MathUtil.h"

namespace Engine
{
	b2WorldId* Physics::m_ptrWorldId = nullptr;
	std::unordered_map<SingleEntityContactKey, std::function<void(BeginContact)>> Physics::m_onBeginContactSingleEntityMap;
	std::unordered_map<SingleEntityContactKey, std::function<void(EndContact)>> Physics::m_onEndContactSingleEntityMap;
	std::unordered_map<SingleEntityContactKey, std::function<void(BeginSensing)>> Physics::m_onBeginSensingSingleEntityMap;
	std::unordered_map<SingleEntityContactKey, std::function<void(EndSensing)>> Physics::m_onEndSensingSingleEntityMap;
	std::unordered_map<MultiEntityContactKey, std::function<void(BeginContact)>> Physics::m_onBeginContactMultiEntityMap;
	std::unordered_map<MultiEntityContactKey, std::function<void(EndContact)>> Physics::m_onEndContactMultiEntityMap;
	std::unordered_map<MultiEntityContactKey, std::function<void(BeginSensing)>> Physics::m_onBeginSensingMultiEntityMap;
	std::unordered_map<MultiEntityContactKey, std::function<void(EndSensing)>> Physics::m_onEndSensingMultiEntityMap;

	// Physics System
	void Physics::SetPosition(Entity* ptrEntity, Vector2D<float> position)
	{
		b2BodyId bodyId = *ECS::GetComponentManager<PhysicsBody>().GetComponent(ptrEntity)->m_bodyId;
		b2Rot rotation = b2Body_GetRotation(bodyId);
		b2Body_SetTransform(bodyId, b2Vec2(position.X, position.Y), rotation);
	}

	const Vector2D<float> Physics::GetPosition(Entity* ptrEntity)
	{
		b2BodyId bodyId = *ECS::GetComponentManager<PhysicsBody>().GetComponent(ptrEntity)->m_bodyId;
		b2Vec2 position = b2Body_GetPosition(bodyId);
		return Vector2D<float>(position.x, position.y);
	}

	const Vector2D<float> Physics::GetTopLeftPosition(Entity* ptrEntity)
	{
		b2BodyId bodyId = *ECS::GetComponentManager<PhysicsBody>().GetComponent(ptrEntity)->m_bodyId;
		b2Vec2 position = b2Body_GetPosition(bodyId);
		PhysicsBody* ptrPhysicsBody = ECS::GetComponentManager<PhysicsBody>().GetComponent(ptrEntity);
		return Vector2D<float>(position.x - ptrPhysicsBody->m_halfDimensions.X, position.y - ptrPhysicsBody->m_halfDimensions.Y);
	}

	void Physics::ApplyForceToBody(Entity* ptrEntity, Vector2D<float> force)
	{
		b2BodyId bodyId = *ECS::GetComponentManager<PhysicsBody>().GetComponent(ptrEntity)->m_bodyId;
		b2Body_ApplyForceToCenter(bodyId, b2Vec2(force.X, force.Y), true);
	}

	void Physics::ApplyImpulseToBody(Entity* ptrEntity, Vector2D<float> impulse)
	{
		b2BodyId bodyId = *ECS::GetComponentManager<PhysicsBody>().GetComponent(ptrEntity)->m_bodyId;
		b2Body_ApplyLinearImpulseToCenter(bodyId, b2Vec2(impulse.X, impulse.Y), true);
	}

	// PhysicsBody2d getter and setter wrappers
	void Physics::SetGravity(Entity* ptrEntity, bool enabled)
	{
		PhysicsBody* ptrPhysicsBody = ECS::GetComponentManager<PhysicsBody>().GetComponent(ptrEntity);
		ptrPhysicsBody->m_gravityOn = enabled;
		b2BodyId bodyId = *ptrPhysicsBody->m_bodyId;
		b2Body_SetGravityScale(bodyId, enabled ? 1.0f : 0.0f);

	}

	void Physics::SetDeceleration(Entity* ptrEntity, const float decel)
	{
		b2BodyId bodyId = *ECS::GetComponentManager<PhysicsBody>().GetComponent(ptrEntity)->m_bodyId;
		b2Body_SetLinearDamping(bodyId, decel);

	}

	void Physics::SetVelocity(Entity* ptrEntity, const Vector2D<float> velocity)
	{
		b2BodyId bodyId = *ECS::GetComponentManager<PhysicsBody>().GetComponent(ptrEntity)->m_bodyId;
		b2Body_SetLinearVelocity(bodyId, b2Vec2(velocity.X, velocity.Y));
	}

	void Physics::SetXVelocity(Entity* ptrEntity, const float xVel)
	{
		b2BodyId bodyId = *ECS::GetComponentManager<PhysicsBody>().GetComponent(ptrEntity)->m_bodyId;
		b2Vec2 velocity = b2Body_GetLinearVelocity(bodyId);
		b2Body_SetLinearVelocity(bodyId, b2Vec2(xVel, velocity.y));
	}

	void Physics::SetYVelocity(Entity* ptrEntity, const float yVel)
	{
		b2BodyId bodyId = *ECS::GetComponentManager<PhysicsBody>().GetComponent(ptrEntity)->m_bodyId;
		b2Vec2 velocity = b2Body_GetLinearVelocity(bodyId);
		b2Body_SetLinearVelocity(bodyId, b2Vec2(velocity.x, yVel));
	}

	const Vector2D<float> Physics::GetVelocity(Entity* ptrEntity)
	{
		b2BodyId bodyId = *ECS::GetComponentManager<PhysicsBody>().GetComponent(ptrEntity)->m_bodyId;
		b2Vec2 velocity = b2Body_GetLinearVelocity(bodyId);
		return Vector2D<float>(velocity.x, velocity.y);
	}

	void Physics::SetRestitution(Entity* ptrEntity, const float restitution)
	{
		b2ShapeId shapeId = *ECS::GetComponentManager<PhysicsBody>().GetComponent(ptrEntity)->m_shapeId;
		b2Shape_SetRestitution(shapeId, restitution);
	}

	void Physics::SetDensity(Entity* ptrEntity, const float density)
	{
		b2ShapeId shapeId = *ECS::GetComponentManager<PhysicsBody>().GetComponent(ptrEntity)->m_shapeId;
		b2Shape_SetDensity(shapeId, density, true);
	}

	void Physics::SetFriction(Entity* ptrEntity, const float friction)
	{
		b2ShapeId shapeId = *ECS::GetComponentManager<PhysicsBody>().GetComponent(ptrEntity)->m_shapeId;
		b2Shape_SetFriction(shapeId, friction);
	}

	void Physics::RemoveBodyFromWorld(Entity* ptrEntity)
	{
		PhysicsBody* ptrPhysicsBody = ECS::GetComponentManager<PhysicsBody>().GetComponent(ptrEntity);
		if (ptrPhysicsBody->m_bodyId == nullptr) return;

		b2DestroyBody(*ptrPhysicsBody->m_bodyId);
		ptrPhysicsBody->m_bodyId = nullptr;
		ptrPhysicsBody->m_shapeId = nullptr;
		ptrPhysicsBody->m_worldId = nullptr;
	}

	void Physics::SetFixedRotation(Entity* ptrEntity, bool fixed)
	{
		b2BodyId bodyId = *ECS::GetComponentManager<PhysicsBody>().GetComponent(ptrEntity)->m_bodyId;
		b2Body_SetFixedRotation(bodyId, fixed);
	}

	const float Physics::GetAngleInRadians(Entity* ptrEntity)
	{
		b2BodyId bodyId = *ECS::GetComponentManager<PhysicsBody>().GetComponent(ptrEntity)->m_bodyId;
		b2Rot rotation = b2Body_GetRotation(bodyId);
		float angleInRadians = b2Rot_GetAngle(rotation);
		return angleInRadians;
	}

	const float Physics::GetAngleInDegrees(Entity* ptrEntity)
	{
		b2BodyId bodyId = *ECS::GetComponentManager<PhysicsBody>().GetComponent(ptrEntity)->m_bodyId;
		b2Rot rotation = b2Body_GetRotation(bodyId);
		float angleInRadians = b2Rot_GetAngle(rotation);
		return angleInRadians * 180.0f / 3.14159265359f;

	}

	void Physics::AddPhysicsBodiesToWorld()
	{
		ComponentManager<PhysicsBody>& physicsBodyManager = ECS::GetComponentManager<PhysicsBody>();
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

	void Physics::Update()
	{
		b2World_Step(*m_ptrWorldId, Time::GetTimeStep(), 4);
		ProcessContactEvents();
	}

	void Physics::ProcessContactEvents()
	{
		// Process ContactComponents
		for (SimpleContact& simpleContact : ECS::GetComponentManager<SimpleContact>())
		{
			simpleContact.m_contactAbove = false;
			simpleContact.m_contactBelow = false;
			simpleContact.m_contactLeft = false;
			simpleContact.m_contactRight = false;

			// Better way to access. Maybe can just store shapeId directly in SimpleContact component?
			Entity* ptrEntity = simpleContact.GetEntity();
			b2ShapeId* shapeId = ECS::GetComponentManager<PhysicsBody>().GetComponent(ptrEntity)->m_shapeId;

			b2ContactData contactData[10];
			int shapeContactCount = b2Shape_GetContactData(*shapeId, contactData, 10);

			for (int i = 0; i < shapeContactCount; ++i)
			{
				b2ContactData* contact = contactData + i;

				float normalDirection = 1.0f;

				if ((Entity*)b2Body_GetUserData(b2Shape_GetBody(contact->shapeIdB)) == ptrEntity) 
				{
					normalDirection = -1.0f;
				}

				b2Vec2 normal = contact->manifold.normal * normalDirection;

				if (normal.y < -0.5) // Collision from above `this`
				{
					// ENGINE_CRITICAL_D("Contact Above!");
					simpleContact.m_contactAbove = true;
				}
				else if (normal.y > 0.5) // Collision from below `this`
				{
					// ENGINE_CRITICAL_D("Contact Below!");
					simpleContact.m_contactBelow = true;
				}

				if (normal.x > 0.5) // Collision from the Right of `this`
				{
					// ENGINE_CRITICAL_D("Contact Right!");
					simpleContact.m_contactRight = true;
				}
				else if (normal.x < -0.5) // Collision from the Left of `this`
				{
					// ENGINE_CRITICAL_D("Contact Left!");
					simpleContact.m_contactLeft = true;
				}
			} 
		}


		// Is there a point to having event listeners be part of the ECS? Consider they fire when an event happens? Not sequentially in a component manager.
		// Process ContactListeners

		// May need to ensure that only one contact event is created per contact. 
		// So that we don't have multiple events for the same contact.
		b2ContactEvents contactEvents = b2World_GetContactEvents(*m_ptrWorldId);

		for (int i = 0; i < contactEvents.beginCount; ++i)
		{
			b2ContactBeginTouchEvent* beginEvent = contactEvents.beginEvents + i;

			b2ShapeId shapeIdA = beginEvent->shapeIdA;
			b2ShapeId shapeIdB = beginEvent->shapeIdB;

			Entity* entityA = (Entity*)b2Body_GetUserData(b2Shape_GetBody(shapeIdA));
			Entity* entityB = (Entity*)b2Body_GetUserData(b2Shape_GetBody(shapeIdB));

			// normal points from A to B
			Vector2D<float> normal = Vector2D(beginEvent->manifold.normal.x, beginEvent->manifold.normal.y);

			auto beginSingleContactIterator = m_onBeginContactSingleEntityMap.find(SingleEntityContactKey(entityA));
			if (beginSingleContactIterator != m_onBeginContactSingleEntityMap.end())
			{
				beginSingleContactIterator->second(BeginContact(entityA, entityB, normal));
			}

			auto multiContactIterator = m_onBeginContactMultiEntityMap.find(MultiEntityContactKey(entityA, entityB));
			if (multiContactIterator != m_onBeginContactMultiEntityMap.end())
			{
				multiContactIterator->second(BeginContact(entityA, entityB, normal));
			}
		}

		for (int i = 0; i < contactEvents.endCount; ++i)
		{
			b2ContactEndTouchEvent* endEvent = contactEvents.endEvents + i;

			b2ShapeId shapeIdA = endEvent->shapeIdA;
			b2ShapeId shapeIdB = endEvent->shapeIdB;

			Entity* entityA = (Entity*)b2Body_GetUserData(b2Shape_GetBody(shapeIdA));
			Entity* entityB = (Entity*)b2Body_GetUserData(b2Shape_GetBody(shapeIdB));

			auto endSingleContactIterator = m_onEndContactSingleEntityMap.find(SingleEntityContactKey(entityA));
			if (endSingleContactIterator != m_onEndContactSingleEntityMap.end())
			{
				endSingleContactIterator->second(EndContact(entityA, entityB));
			}

			auto endMultiContactIterator = m_onEndContactMultiEntityMap.find(MultiEntityContactKey(entityA, entityB));
			if (endMultiContactIterator != m_onEndContactMultiEntityMap.end())
			{
				endMultiContactIterator->second(EndContact(entityA, entityB));
			}
		}

		///*for (int i = 0; i < contactEvents.hitCount; ++i)
		//{
		//	b2ContactHitEvent* hitEvent = contactEvents.hitEvents + i;\
		//}*/

		b2SensorEvents sensorEvents = b2World_GetSensorEvents(*m_ptrWorldId);

		for (int i = 0; i < sensorEvents.beginCount; ++i)
		{
			b2SensorBeginTouchEvent* beginEvent = sensorEvents.beginEvents + i;

			b2ShapeId shapeIdA = beginEvent->visitorShapeId;
			b2ShapeId shapeIdB = beginEvent->sensorShapeId;

			Entity* entityA = (Entity*)b2Body_GetUserData(b2Shape_GetBody(shapeIdA));
			Entity* entityB = (Entity*)b2Body_GetUserData(b2Shape_GetBody(shapeIdB));

			auto beginSingleSensingIterator = m_onBeginSensingSingleEntityMap.find(SingleEntityContactKey(entityA));
			if (beginSingleSensingIterator != m_onBeginSensingSingleEntityMap.end())
			{
				beginSingleSensingIterator->second(BeginSensing(entityA, entityB));
			}

			auto beginMultiSensingIterator = m_onBeginSensingMultiEntityMap.find(MultiEntityContactKey(entityA, entityB));
			if (beginMultiSensingIterator != m_onBeginSensingMultiEntityMap.end())
			{
				beginMultiSensingIterator->second(BeginSensing(entityA, entityB));
			}
		}

		for (int i = 0; i < sensorEvents.endCount; ++i)
		{
			b2SensorEndTouchEvent* endEvent = sensorEvents.endEvents + i;

			b2ShapeId shapeIdA = endEvent->visitorShapeId;
			b2ShapeId shapeIdB = endEvent->sensorShapeId;

			Entity* entityA = (Entity*)b2Body_GetUserData(b2Shape_GetBody(shapeIdA));
			Entity* entityB = (Entity*)b2Body_GetUserData(b2Shape_GetBody(shapeIdB));

			auto endSingleSensingIterator = m_onEndSensingSingleEntityMap.find(SingleEntityContactKey(entityA));
			if (endSingleSensingIterator != m_onEndSensingSingleEntityMap.end())
			{
				endSingleSensingIterator->second(EndSensing(entityA, entityB));
			}

			auto endMultiSensingIterator = m_onEndSensingMultiEntityMap.find(MultiEntityContactKey(entityA, entityB));
			if (endMultiSensingIterator != m_onEndSensingMultiEntityMap.end())
			{
				endMultiSensingIterator->second(EndSensing(entityA, entityB));
			}
		}
	}

	void Physics::RegisterOnBeginContactEventListener(SingleEntityContactKey key, std::function<void(BeginContact)> callback)
	{
		m_onBeginContactSingleEntityMap[key] = callback;
	}

	void Physics::RegisterOnEndContactEventListener(SingleEntityContactKey key, std::function<void(EndContact)> callback)
	{
		m_onEndContactSingleEntityMap[key] = callback;
	}

	void Physics::RegisterOnBeginSensingEventListener(SingleEntityContactKey key, std::function<void(BeginSensing)> callback)
	{
		m_onBeginSensingSingleEntityMap[key] = callback;
	}

	void Physics::RegisterOnEndSensingEventListener(SingleEntityContactKey key, std::function<void(EndSensing)> callback)
	{
		m_onEndSensingSingleEntityMap[key] = callback;
	}

	void Physics::RegisterOnBeginContactEventListener(MultiEntityContactKey key, std::function<void(BeginContact)> callback)
	{
		m_onBeginContactMultiEntityMap[key] = callback;
	}

	void Physics::RegisterOnEndContactEventListener(MultiEntityContactKey key, std::function<void(EndContact)> callback)
	{
		m_onEndContactMultiEntityMap[key] = callback;

	}

	void Physics::RegisterOnBeginSensingEventListener(MultiEntityContactKey key, std::function<void(BeginSensing)> callback)
	{
		m_onBeginSensingMultiEntityMap[key] = callback;
	}
	
	void Physics::RegisterOnEndSensingEventListener(MultiEntityContactKey key, std::function<void(EndSensing)> callback)
	{
		m_onEndSensingMultiEntityMap[key] = callback;
	}

	void Physics::CreateWorld(const Vector2D<float> gravity)
	{
		if (m_ptrWorldId != nullptr)
		{
			ENGINE_INFO_D("World already exists.");
			return;
		}

		ENGINE_INFO_D("Creating World!");

		b2WorldDef worldDef = b2DefaultWorldDef();
		worldDef.gravity = { gravity.X, gravity.Y };
		m_ptrWorldId = new b2WorldId(b2CreateWorld(&worldDef));

		ENGINE_INFO_D("World created!");
	}

	void Physics::DestroyWorld()
	{
		if (m_ptrWorldId == nullptr)
		{
			ENGINE_INFO_D("World is already null. No need to destroy.");
			return;
		}

		ENGINE_INFO_D("Freeing World!");

		b2DestroyWorld(*m_ptrWorldId);

		for (PhysicsBody& refPhysicsBody : ECS::GetComponentManager<PhysicsBody>())
		{
			refPhysicsBody.m_bodyId = nullptr;
			refPhysicsBody.m_shapeId = nullptr;
			refPhysicsBody.m_worldId = nullptr;
		}

		delete m_ptrWorldId;
		m_ptrWorldId = nullptr;
		ENGINE_INFO_D("World freed!");
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