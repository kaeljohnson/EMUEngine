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

	void PhysicsInterface::CreateBody(Entity entity)
	{
		m_refECS.AddComponent<PhysicsBody>(entity);
	}

	const bool PhysicsInterface::HasBody(Entity entity)
	{
		return m_refECS.HasComponent<PhysicsBody>(entity);
	}

	void PhysicsInterface::SetBodyType(Entity entity, const BodyType type)
	{
		GetBody(entity)->m_bodyType = type;
	}

	void PhysicsInterface::SetDimensions(Entity entity, const Vector2D<float> dimensions)
	{
		PhysicsBody* ptrBody = GetBody(entity);
		ptrBody->m_dimensions = dimensions;
		ptrBody->m_halfDimensions = dimensions / 2.0f;
	}

	const Vector2D<float> PhysicsInterface::GetDimensions(Entity entity)
	{
		return GetBody(entity)->m_dimensions;
	}

	const Vector2D<float> PhysicsInterface::GetDimensions(PhysicsBody& body)
	{
		return body.m_dimensions;
	}

	PhysicsBody* PhysicsInterface::GetBody(Entity entity)
	{
		return m_refECS.GetComponent<PhysicsBody>(entity);
	}

	void PhysicsInterface::SetStartingPosition(Entity entity, Vector2D<float> position)
	{
		PhysicsBody* ptrBody = GetBody(entity);
		ptrBody->m_startingPosition = position;
	}

	void PhysicsInterface::SetPosition(Entity entity, Vector2D<float> position)
	{
		b2BodyId bodyId = *GetBody(entity)->m_bodyId;
		b2Rot rotation = b2Body_GetRotation(bodyId);
		b2Body_SetTransform(bodyId, b2Vec2(position.X, position.Y), rotation);
	}

	const Vector2D<float> PhysicsInterface::GetPosition(Entity entity)
	{
		return GetBody(entity)->m_position;
	}

	const Vector2D<float> PhysicsInterface::GetTopLeftPosition(Entity entity)
	{
		PhysicsBody* ptrBody = GetBody(entity);
		b2Vec2 position = b2Body_GetPosition(*ptrBody->m_bodyId);
		return Vector2D<float>(position.x - ptrBody->m_halfDimensions.X, position.y - ptrBody->m_halfDimensions.Y);
	}

	void PhysicsInterface::ApplyForceToBody(Entity entity, Vector2D<float> force)
	{
		b2BodyId bodyId = *GetBody(entity)->m_bodyId;
		b2Body_ApplyForceToCenter(bodyId, b2Vec2(force.X, force.Y), true);
	}

	void PhysicsInterface::ApplyImpulseToBody(Entity entity, Vector2D<float> impulse)
	{
		b2BodyId bodyId = *GetBody(entity)->m_bodyId;
		b2Body_ApplyLinearImpulseToCenter(bodyId, b2Vec2(impulse.X, impulse.Y), true);
	}

	// PhysicsBody2d getter and setter wrappers
	void PhysicsInterface::SetGravity(Entity entity, bool enabled)
	{
		PhysicsBody* ptrBody = GetBody(entity);
		ptrBody->m_gravityOn = enabled;
		b2BodyId bodyId = *ptrBody->m_bodyId;
		b2Body_SetGravityScale(bodyId, enabled ? 1.0f : 0.0f);

	}

	void PhysicsInterface::SetDeceleration(Entity entity, const float decel)
	{
		b2BodyId bodyId = *GetBody(entity)->m_bodyId;
		b2Body_SetLinearDamping(bodyId, decel);

	}

	void PhysicsInterface::SetVelocity(Entity entity, const Vector2D<float> velocity)
	{
		b2BodyId bodyId = *GetBody(entity)->m_bodyId;
		b2Body_SetLinearVelocity(bodyId, b2Vec2(velocity.X, velocity.Y));
	}

	void PhysicsInterface::SetXVelocity(Entity entity, const float xVel)
	{
		b2BodyId bodyId = *GetBody(entity)->m_bodyId;
		b2Vec2 velocity = b2Body_GetLinearVelocity(bodyId);
		b2Body_SetLinearVelocity(bodyId, b2Vec2(xVel, velocity.y));
	}

	void PhysicsInterface::SetYVelocity(Entity entity, const float yVel)
	{
		b2BodyId bodyId = *GetBody(entity)->m_bodyId;
		b2Vec2 velocity = b2Body_GetLinearVelocity(bodyId);
		b2Body_SetLinearVelocity(bodyId, b2Vec2(velocity.x, yVel));
	}

	const Vector2D<float> PhysicsInterface::GetVelocity(Entity entity)
	{
		b2BodyId bodyId = *GetBody(entity)->m_bodyId;
		b2Vec2 velocity = b2Body_GetLinearVelocity(bodyId);
		return Vector2D<float>(velocity.x, velocity.y);
	}

	void PhysicsInterface::SetRestitution(Entity entity, const float restitution)
	{
		b2ShapeId shapeId = *GetBody(entity)->m_shapeId;
		b2Shape_SetRestitution(shapeId, restitution);
	}

	void PhysicsInterface::SetDensity(Entity entity, const float density)
	{
		b2ShapeId shapeId = *GetBody(entity)->m_shapeId;
		b2Shape_SetDensity(shapeId, density, true);
	}

	void PhysicsInterface::SetFriction(Entity entity, const float friction)
	{
		b2ShapeId shapeId = *GetBody(entity)->m_shapeId;
		b2Shape_SetFriction(shapeId, friction);
	}

	void PhysicsInterface::RemoveBodyFromWorld(Entity entity)
	{
		PhysicsBody* ptrBody = GetBody(entity);
		if (ptrBody->m_bodyId == nullptr) return;

		b2DestroyBody(*ptrBody->m_bodyId);
		ptrBody->m_bodyId = nullptr;
		ptrBody->m_shapeId = nullptr;
		ptrBody->m_worldId = nullptr;
	}

	void PhysicsInterface::SetFixedRotation(Entity entity, bool fixed)
	{
		b2BodyId bodyId = *GetBody(entity)->m_bodyId;
		b2Body_SetFixedRotation(bodyId, fixed);
	}

	const float PhysicsInterface::GetAngleInRadians(Entity entity)
	{
		b2BodyId bodyId = *GetBody(entity)->m_bodyId;
		b2Rot rotation = b2Body_GetRotation(bodyId);
		float angleInRadians = b2Rot_GetAngle(rotation);
		return angleInRadians;
	}

	const float PhysicsInterface::GetAngleInDegrees(Entity entity)
	{
		b2BodyId bodyId = *GetBody(entity)->m_bodyId;
		b2Rot rotation = b2Body_GetRotation(bodyId);
		float angleInRadians = b2Rot_GetAngle(rotation);
		return angleInRadians * 180.0f / 3.14159265359f;

	}

	const bool PhysicsInterface::HasContactBelow(Entity entity)
	{
		PhysicsBody* ptrBody = GetBody(entity);
		return ptrBody->m_contactBelow;
	}

	const bool PhysicsInterface::HasContactAbove(Entity entity)
	{
		PhysicsBody* ptrBody = GetBody(entity);
		return ptrBody->m_contactAbove;
	}

	const bool PhysicsInterface::HasContactLeft(Entity entity)
	{
		PhysicsBody* ptrBody = GetBody(entity);
		return ptrBody->m_contactLeft;
	}

	const bool PhysicsInterface::HasContactRight(Entity entity)
	{
		PhysicsBody* ptrBody = GetBody(entity);
		return ptrBody->m_contactRight;
	}

	// Physics Simulation
	PhysicsSimulation::PhysicsSimulation(ECS& refECS, TileMap& tileMap)
		: m_refECS(refECS), m_ptrWorldId(nullptr), m_contactSystem(refECS, tileMap), m_gravity(0.0f, 9.81f)
	{}

	void PhysicsSimulation::CreateWorld()
	{
		ENGINE_INFO_D("Creating World!");

		b2WorldDef worldDef = b2DefaultWorldDef();
		worldDef.gravity = { m_gravity.X, m_gravity.Y };
		m_ptrWorldId = new b2WorldId(b2CreateWorld(&worldDef));

		ENGINE_INFO_D("World created!");
	}

	void PhysicsSimulation::UpdateGravity(const Vector2D<float> gravity)
	{
		if (AppState::IN_SCENE)
		{
			b2WorldDef worldDef = b2DefaultWorldDef();
			worldDef.gravity = { gravity.X, gravity.Y };
			b2World_SetGravity(*m_ptrWorldId, worldDef.gravity);
		}
		else
		{
			m_gravity = gravity;
		}
	}

	void PhysicsSimulation::AddPhysicsBodiesToWorld(std::vector<Entity>& entities)
	{
		for (Entity& entitiy : entities)
		{
			if (!m_refECS.HasComponent<PhysicsBody>(entitiy)) continue;

			PhysicsBody& refPhysicsBody = *m_refECS.GetComponent<PhysicsBody>(entitiy);
			AddPhysicsBodyToWorld(refPhysicsBody);
		}
	}

	void PhysicsSimulation::AddPhysicsBodyToWorld(PhysicsBody& refPhysicsBody)
	{
		if (refPhysicsBody.m_bodyId != nullptr)
		{
			ENGINE_INFO_D("Body already exists. Not adding to world.");
			return;
		}

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

		bodyDef.isEnabled = refPhysicsBody.m_enabled;
		bodyDef.fixedRotation = true;
		bodyDef.userData = (void*)refPhysicsBody.m_entity; // NOTE: This is the entity of the physics body. Not a pointer to the physics body.

		b2BodyId bodyId = b2CreateBody(*m_ptrWorldId, &bodyDef);

		shapeDef.density = 1.0f;
		shapeDef.friction = 0.0f;
		shapeDef.restitution = 0.0f;

		// Set collision filters. Default to all for now.
		shapeDef.filter.categoryBits = refPhysicsBody.m_category;
		shapeDef.filter.maskBits = refPhysicsBody.m_mask;

		b2Polygon box = b2MakeBox(refPhysicsBody.m_halfDimensions.X, refPhysicsBody.m_halfDimensions.Y);
		b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box);

		refPhysicsBody.m_bodyId = new b2BodyId(bodyId);
		refPhysicsBody.m_shapeId = new b2ShapeId(shapeId);
		refPhysicsBody.m_worldId = m_ptrWorldId;
	}

	void PhysicsSimulation::AddLineCollidersToWorld(std::vector<Entity>& entities)
	{
		// This function also needs to connect lines together that meet at the same point.
		ENGINE_INFO_D("Hot chains size! " + std::to_string(m_refECS.GetHotComponents<ChainCollider>().size()));

		auto createChain = [&](ChainCollider* ptrChain)
			{
				b2ChainDef chainDef = b2DefaultChainDef();
				b2BodyDef bodyDef = b2DefaultBodyDef();

				chainDef.count = 4;
				chainDef.filter.categoryBits = ptrChain->m_category;
				chainDef.filter.maskBits = ptrChain->m_mask;
				chainDef.friction = 0.0f;
				chainDef.restitution = 0.0f;
				chainDef.isLoop = false;

				b2Vec2 points[4];
				for (int i = 0; i < 4; ++i)
				{
					points[i].x = ptrChain->m_points[i].X;
					points[i].y = ptrChain->m_points[i].Y;
				}

				chainDef.points = points;
				chainDef.userData = (void*)ptrChain->m_entity;

				bodyDef.isEnabled = ptrChain->m_enabled;

				bodyDef.type = b2_staticBody;
				bodyDef.fixedRotation = true;
				bodyDef.userData = (void*)ptrChain->m_entity;

				b2BodyId bodyId = b2CreateBody(*m_ptrWorldId, &bodyDef);
				b2ChainId chainId = b2CreateChain(bodyId, &chainDef);

				ptrChain->m_bodyId = new b2BodyId(bodyId);
				ptrChain->m_chainId = new b2ChainId(chainId);
				ptrChain->m_worldId = m_ptrWorldId;
			};

		for (auto& entity : entities)
		{
			if (m_refECS.HasComponent<ChainColliderLeft>(entity))
				createChain(m_refECS.GetComponent<ChainColliderLeft>(entity));

			if (m_refECS.HasComponent<ChainColliderRight>(entity))
				createChain(m_refECS.GetComponent<ChainColliderRight>(entity));

			if (m_refECS.HasComponent<ChainColliderTop>(entity))
				createChain(m_refECS.GetComponent<ChainColliderTop>(entity));

			if (m_refECS.HasComponent<ChainColliderBottom>(entity))
				createChain(m_refECS.GetComponent<ChainColliderBottom>(entity));
		}
	}

	void PhysicsSimulation::ActivateContactCallbacks()
	{
		m_contactSystem.ActivateContactCallbacks();
	}

	void PhysicsSimulation::ProcessSimpleContacts(PhysicsBody& refPhysicsBody)
	{
		refPhysicsBody.m_contactAbove = false;
		refPhysicsBody.m_contactBelow = false;
		refPhysicsBody.m_contactLeft = false;
		refPhysicsBody.m_contactRight = false;

		// Better way to access. Maybe can just store shapeId directly in SimpleContact component?
		Entity entity = refPhysicsBody.m_entity;
		b2ShapeId* shapeId = m_refECS.GetComponent<PhysicsBody>(entity)->m_shapeId;

		b2ContactData contactData[10];
		int shapeContactCount = b2Shape_GetContactData(*shapeId, contactData, 10);

		for (int i = 0; i < shapeContactCount; ++i)
		{
			b2ContactData* contact = contactData + i;

			float normalDirection = 1.0f;

			if ((Entity)b2Body_GetUserData(b2Shape_GetBody(contact->shapeIdB)) == entity)
			{
				normalDirection = -1.0f;
			}

			b2Vec2 normal = contact->manifold.normal * normalDirection;

			if (normal.y < -0.5) // Collision from above `this`
			{
				// ENGINE_CRITICAL_D("Contact Above!");
				refPhysicsBody.m_contactAbove = true;
			}
			else if (normal.y > 0.5) // Collision from below `this`
			{
				// ENGINE_CRITICAL_D("Contact Below!");
				refPhysicsBody.m_contactBelow = true;
			}
			
			if (normal.x > 0.5) // Collision from the Right of `this`
			{
				// ENGINE_CRITICAL_D("Contact Right!");
				refPhysicsBody.m_contactRight = true;
			}
			else if (normal.x < -0.5) // Collision from the Left of `this`
			{
				// ENGINE_CRITICAL_D("Contact Left!");
				refPhysicsBody.m_contactLeft = true;
			}
		}
	}

	void PhysicsSimulation::Update()
	{
		 // auto start = std::chrono::high_resolution_clock::now();

		b2World_Step(*m_ptrWorldId, Time::GetTimeStep(), 4);
		m_contactSystem.ProcessContacts(m_ptrWorldId);

		auto& hotPhysicsBodies = m_refECS.GetHotComponents<PhysicsBody>(); // Worth storing a ref to these at beginning of scene?

		for (PhysicsBody& refPhysicsBody : hotPhysicsBodies) // Update physics bodies first since all bodies need updating but not all entities with bodies have transforms.
		{
			if (refPhysicsBody.m_checkSimpleContacts) ProcessSimpleContacts(refPhysicsBody);

			// Static bodies don't move, so no need to update their position/rotation.
			if (refPhysicsBody.m_bodyType == STATIC) continue;

			b2Vec2 position = b2Body_GetPosition(*refPhysicsBody.m_bodyId);
			refPhysicsBody.m_position = Vector2D<float>(position.x - refPhysicsBody.m_halfDimensions.X, position.y - refPhysicsBody.m_halfDimensions.Y);

			b2Rot rotation = b2Body_GetRotation(*refPhysicsBody.m_bodyId);
			refPhysicsBody.m_rotation = b2Rot_GetAngle(rotation) * 180.0f / 3.14159265359f;

			if (Transform* ptrTransform = m_refECS.GetComponent<Transform>(refPhysicsBody.m_entity)) // Don't worry about transform being inactive as we already know its corresponding physics body is active.
			{
				ptrTransform->PrevPosition = ptrTransform->Position;
				ptrTransform->Dimensions = refPhysicsBody.m_dimensions;
				ptrTransform->Position = refPhysicsBody.m_position;
				ptrTransform->Rotation = refPhysicsBody.m_rotation;
			}

			// Is this the best place for this?
			PhysicsUpdater* ptrPhysicsUpdater = m_refECS.GetComponent<PhysicsUpdater>(refPhysicsBody.m_entity);
			if (ptrPhysicsUpdater)
				ptrPhysicsUpdater->Update(refPhysicsBody.m_entity);
		}

		// auto end = std::chrono::high_resolution_clock::now();
		// std::chrono::duration<double, std::milli> elapsed = end - start;
		// ENGINE_INFO_D("Physics step time: " + std::to_string(elapsed.count()) + " ms");
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

		for (PhysicsBody& refPhysicsBody : m_refECS.GetHotComponents<PhysicsBody>())
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

	// Change these next four functions to be calle "Runtime-"
	void PhysicsSimulation::DeactivateBody(Entity entity)
	{
		if (AppState::IN_SCENE)
		{
			if (m_refECS.HasComponent<PhysicsBody>(entity))
			{
				// Remove the body from the world
				PhysicsBody* ptrBody = m_refECS.GetComponent<PhysicsBody>(entity);
				if (ptrBody->m_bodyId != nullptr)
				{
					b2Body_Disable(*ptrBody->m_bodyId);
					ptrBody->m_enabled = false;
				}
				else 
				{
					ENGINE_INFO_D("Physics body is null. Cannot remove from world.");
					return;
				}

				// Deactivate the body
				m_refECS.DeactivateComponent<PhysicsBody>(entity);
			}
		}
	}

	void PhysicsSimulation::ActivateBody(Entity entity)
	{
		if (AppState::IN_SCENE)
		{
			ENGINE_CRITICAL_D("Activating body: " + std::to_string(entity));
			// bug here: If it wasn't a part of the scene, it will have to be added to the world here.
			if (m_refECS.HasComponent<PhysicsBody>(entity))
			{
				PhysicsBody* ptrBody = m_refECS.GetComponent<PhysicsBody>(entity);
				b2Body_Enable(*ptrBody->m_bodyId);
				ptrBody->m_enabled = true;
			}
		}
	}

	void PhysicsSimulation::ActivateChains(Entity entity)
	{
		if (AppState::IN_SCENE)
		{
			if (m_refECS.HasComponent<ChainColliderLeft>(entity))
			{
				ChainColliderLeft* ptrChainColliderLeft = m_refECS.GetComponent<ChainColliderLeft>(entity);
				b2Body_Enable(*ptrChainColliderLeft->m_bodyId);
				ptrChainColliderLeft->m_enabled = true;
			}
			if (m_refECS.HasComponent<ChainColliderRight>(entity))
			{
				ChainColliderRight* ptrChainColliderRight = m_refECS.GetComponent<ChainColliderRight>(entity);
				b2Body_Enable(*ptrChainColliderRight->m_bodyId);
				ptrChainColliderRight->m_enabled = true;
			}
			if (m_refECS.HasComponent<ChainColliderTop>(entity))
			{
				ChainColliderTop* ptrChainColliderTop = m_refECS.GetComponent<ChainColliderTop>(entity);
				b2Body_Enable(*ptrChainColliderTop->m_bodyId);
				ptrChainColliderTop->m_enabled = true;
			}
			if (m_refECS.HasComponent<ChainColliderBottom>(entity))
			{
				ChainColliderBottom* ptrChainColliderBottom = m_refECS.GetComponent<ChainColliderBottom>(entity);
				b2Body_Enable(*ptrChainColliderBottom->m_bodyId);
				ptrChainColliderBottom->m_enabled = true;
			}
		}
	}

	void PhysicsSimulation::DeactivateChains(Entity entity)
	{
		if (AppState::IN_SCENE)
		{
			if (m_refECS.HasComponent<ChainColliderLeft>(entity))
			{
				ChainColliderLeft* ptrChainColliderLeft = m_refECS.GetComponent<ChainColliderLeft>(entity);
				b2Body_Disable(*ptrChainColliderLeft->m_bodyId);
				ptrChainColliderLeft->m_enabled = false;
			}
			if (m_refECS.HasComponent<ChainColliderRight>(entity))
			{
				ChainColliderRight* ptrChainColliderRight = m_refECS.GetComponent<ChainColliderRight>(entity);
				b2Body_Disable(*ptrChainColliderRight->m_bodyId);
				ptrChainColliderRight->m_enabled = false;
			}
			if (m_refECS.HasComponent<ChainColliderTop>(entity))
			{
				ChainColliderTop* ptrChainColliderTop = m_refECS.GetComponent<ChainColliderTop>(entity);
				b2Body_Disable(*ptrChainColliderTop->m_bodyId);
				ptrChainColliderTop->m_enabled = false;
			}
			if (m_refECS.HasComponent<ChainColliderBottom>(entity))
			{
				ChainColliderBottom* ptrChainColliderBottom = m_refECS.GetComponent<ChainColliderBottom>(entity);
				b2Body_Disable(*ptrChainColliderBottom->m_bodyId);
				ptrChainColliderBottom->m_enabled = false;
			}
		}
	}
}