//#pragma once
//
//#include <utility>
//#include "../../include/MathUtil.h"
//
//#include "../../include/Physics/PhysicsBody.h"
//#include "../../include/Logging/Logger.h"
//#include "../../include/Physics/BodyTypes.h"
//#include "../../include/ECS/ECS.h"
//#include "../../include/GameState.h"
//
//#include "box2d/box2d.h"
//
//namespace Engine
//{
//	PhysicsBody::PhysicsBody(Entity* entity) : m_bodyID(nullptr), m_worldID(nullptr), m_shapeID(nullptr), 
//		m_halfWidth(0.0f), m_halfHeight(0.0f), m_width(0.0f), m_height(0.0f),
//		m_startingPosition(Vector2D<float>(0.0f, 0.0f)), m_bodyType(STATIC), m_fixed(false), 
//		m_bottomCollision(false), m_topCollision(false), m_leftCollision(false), m_rightCollision(false),
//		m_bottomSensor(false), m_topSensor(false), m_leftSensor(false), m_rightSensor(false),
//		m_gravityOn(true), m_isSensor(false), m_fixedRotation(true), m_restitution(0.0f),
//		m_restitutionThreshold(0.0f), m_density(1.0f), m_friction(0.0f), Component(entity)
//	
//	{
//	}
//
//	PhysicsBody::PhysicsBody(Entity* entity, const BodyType bodyType, const bool fixed, const Vector2D<float> position, const Vector2D<float> size)
//		: m_bodyID(nullptr), m_worldID(nullptr), m_shapeID(nullptr),
//		m_halfWidth(size.X / 2.0f), m_halfHeight(size.Y / 2.0f), 
//		m_width(size.X), m_height(size.Y), m_startingPosition(position),
//		m_bodyType(bodyType), m_fixed(fixed), 
//		m_bottomCollision(false), m_topCollision(false), m_leftCollision(false), m_rightCollision(false),
//		m_bottomSensor(false), m_topSensor(false), m_leftSensor(false), m_rightSensor(false),
//		m_gravityOn(true), m_isSensor(false), m_fixedRotation(true), m_restitution(0.0f),
//		m_restitutionThreshold(0.0f), m_density(1.0f), m_friction(0.0f),
//		Component(entity)
//	{}
//
//	PhysicsBody::~PhysicsBody()
//	{
//		RemoveBodyFromWorld();
//	}
//
//	void PhysicsBody::RemoveBodyFromWorld()
//	{
//		if (m_bodyID != nullptr)
//		{
//			b2DestroyBody(*m_bodyID);
//			SetContactFlagsToFalse();
//			SetPointersToNull();
//		}
//	}
//
//	void PhysicsBody::SetPointersToNull()
//	{
//		m_bodyID = nullptr;
//		m_worldID = nullptr;
//		m_shapeID = nullptr;
//	}
//
//	void PhysicsBody::ApplyForceToBody(Vector2D<float> force)
//	{
//		b2Body_ApplyForceToCenter(*m_bodyID, b2Vec2(force.X, force.Y), true);
//	}
//
//	void PhysicsBody::ApplyImpulseToBody(Vector2D<float> impulse)
//	{
//		b2Body_ApplyLinearImpulseToCenter(*m_bodyID, b2Vec2(impulse.X, impulse.Y), true);
//	}
//
//	void PhysicsBody::SetStartingDensity(const float density)
//	{
//		m_density = density;
//	}
//
//	void PhysicsBody::SetDensity(const float density)
//	{
//		b2Shape_SetDensity(*m_shapeID, density, true);
//	}
//
//
//	void PhysicsBody::SetStartingRestitution(const float restitution)
//	{
//		m_restitution = restitution;
//	}
//
//	void PhysicsBody::SetRestitution(const float restitution)
//	{
//		b2Shape_SetRestitution(*m_shapeID, restitution);
//	}
//
//	void PhysicsBody::SetStartingFriction(const float friction)
//	{
//		m_friction = friction;
//	}
//
//	void PhysicsBody::SetFriction(const float friction)
//	{
//		b2Shape_SetFriction(*m_shapeID, friction);
//	}
//
//	void PhysicsBody::SetIsSensor(const bool isSensor)
//	{
//		m_isSensor = isSensor;
//	}
//
//	void PhysicsBody::OnDeactivate()
//	{
//		if (GameState::IN_SCENE)
//			b2Body_SetAwake(*m_bodyID, false);
//	}
//
//	void PhysicsBody::OnActivate()
//	{
//		if (GameState::IN_SCENE)
//			b2Body_SetAwake(*m_bodyID, true);
//	}
//
//	void PhysicsBody::AddCollidingBody(Entity* otherEntity, const ContactDirection contactDirection)
//	{
//		m_collidingBodies[otherEntity] = contactDirection;
//	}
//
//	void PhysicsBody::RemoveCollidingBody(Entity* otherEntity)
//	{
//		m_collidingBodies.erase(otherEntity);
//	}
//
//	void PhysicsBody::SetContactFlags()
//	{
//
//		// TEMP WAY TO DO THIS
//		// Problem. flags could be set to false before the other body has a chance to set its flags.
//		SetContactFlagsToFalse();
//
//		for (auto& contactPair : m_collidingBodies)
//		{
//			PhysicsBody* otherPhysicsBody = ECS::GetComponentManager<PhysicsBody>().GetComponent(contactPair.first);
//			ContactDirection contactDirection = contactPair.second;
//
//			if (contactDirection == ContactDirection::TOP)
//			{
//				this->SetTopCollision(true);
//				otherPhysicsBody->SetBottomCollision(true);
//			}
//			else if (contactDirection == ContactDirection::BOTTOM)
//			{
//				this->SetBottomCollision(true);
//				otherPhysicsBody->SetTopCollision(true);
//			}
//			else if (contactDirection == ContactDirection::LEFT)
//			{
//				this->SetLeftCollision(true);
//				otherPhysicsBody->SetRightCollision(true);
//			}
//			else if (contactDirection == ContactDirection::RIGHT)
//			{
//				this->SetRightCollision(true);
//				otherPhysicsBody->SetLeftCollision(true);
//			}
//		}
//
//		for (auto& sensorContact : m_sensorContacts)
//		{
//			PhysicsBody* otherPhysicsBody = ECS::GetComponentManager<PhysicsBody>().GetComponent(sensorContact);
//			if (this->GetCenterPosition().Y < otherPhysicsBody->GetCenterPosition().Y)
//			{
//				this->SetTopSensor(true);
//				otherPhysicsBody->SetBottomSensor(true);
//			}
//			else if (this->GetCenterPosition().Y > otherPhysicsBody->GetCenterPosition().Y)
//			{
//				this->SetBottomSensor(true);
//				otherPhysicsBody->SetTopSensor(true);
//			}
//
//			if (this->GetCenterPosition().X < otherPhysicsBody->GetCenterPosition().X)
//			{
//				this->SetLeftSensor(true);
//				otherPhysicsBody->SetRightSensor(true);
//			}
//			else if (this->GetCenterPosition().X > otherPhysicsBody->GetCenterPosition().X)
//			{
//				this->SetRightSensor(true);
//				otherPhysicsBody->SetLeftSensor(true);
//			}
//		}
//	}
//
//	void PhysicsBody::AddSensorContact(Entity* ptrEntity)
//	{
//		m_sensorContacts.push_back(ptrEntity);
//	}
//
//	void PhysicsBody::RemoveSensorContact(Entity* ptrEntity)
//	{
//		m_sensorContacts.erase(std::remove(m_sensorContacts.begin(), m_sensorContacts.end(), ptrEntity), m_sensorContacts.end());
//	}
//
//	void PhysicsBody::SetContactFlags(const bool leftCollision, const bool topCollision, const bool rightCollision, const bool bottomCollision)
//	{
//		m_leftCollision = leftCollision;
//		m_topCollision = topCollision;
//		m_rightCollision = rightCollision;
//		m_bottomCollision = bottomCollision;
//	}
//
//	void PhysicsBody::SetSensorFlags(const bool leftSensor, const bool topSensor, const bool rightSensor, const bool bottomSensor)
//	{
//		m_leftSensor = leftSensor;
//		m_topSensor = topSensor;
//		m_rightSensor = rightSensor;
//		m_bottomSensor = bottomSensor;
//	}
//
//	void PhysicsBody::SetContactFlagsToFalse()
//	{
//		m_bottomCollision = false;
//		m_topCollision = false;
//		m_leftCollision = false;
//		m_rightCollision = false;
//
//		m_bottomSensor = false;
//		m_topSensor = false;
//		m_leftSensor = false;
//		m_rightSensor = false;
//	}
//
//	void PhysicsBody::Update()
//	{
//		switch (GetBodyType())
//		{
//		case DYNAMIC:
//			SetContactFlags();
//			break;
//		case STATIC:
//			SetContactFlagsToFalse();
//			break;
//		case SENSOR:
//			SetContactFlagsToFalse();
//			break;
//		default:
//			ENGINE_WARN_D("Body type has no engine side updating.");
//			break;
//		}
//	}
//
//	void PhysicsBody::OnUnload()
//	{
//		m_collidingBodies.clear();
//		m_sensorContacts.clear();
//		SetPointersToNull();
//		SetContactFlagsToFalse();
//	}
//
//	void PhysicsBody::SetGravity(bool enabled) { b2Body_SetGravityScale(*m_bodyID, enabled ? 1.0f : 0.0f); }
//
//	void PhysicsBody::SetXVelocity(const float xVel) { b2Body_SetLinearVelocity(*m_bodyID, b2Vec2(xVel, b2Body_GetLinearVelocity(*m_bodyID).y)); }
//	void PhysicsBody::SetYVelocity(const float yVel) { b2Body_SetLinearVelocity(*m_bodyID, b2Vec2(b2Body_GetLinearVelocity(*m_bodyID).x, yVel)); }
//	void PhysicsBody::SetXDeceleration(const float xDecel) { b2Body_SetLinearDamping(*m_bodyID, xDecel); }
//	void PhysicsBody::SetFixedRotation(bool fixed) { b2Body_SetFixedRotation(*m_bodyID, fixed); }
//
//	void PhysicsBody::SetBottomCollision(const bool bottomCollision) { m_bottomCollision = bottomCollision; }
//	void PhysicsBody::SetTopCollision(const bool topCollision) { m_topCollision = topCollision; }
//	void PhysicsBody::SetLeftCollision(const bool leftCollision) { m_leftCollision = leftCollision; }
//	void PhysicsBody::SetRightCollision(const bool rightCollision) { m_rightCollision = rightCollision; }
//
//	void PhysicsBody::SetBottomSensor(const bool bottomSensor) { m_bottomSensor = bottomSensor; }
//	void PhysicsBody::SetTopSensor(const bool topSensor) { m_topSensor = topSensor; }
//	void PhysicsBody::SetLeftSensor(const bool leftSensor) { m_leftSensor = leftSensor; }
//	void PhysicsBody::SetRightSensor(const bool rightSensor) { m_rightSensor = rightSensor; }
//
//	const Vector2D<float> PhysicsBody::GetVelocity() const { return Vector2D<float>(b2Body_GetLinearVelocity(*m_bodyID).x, b2Body_GetLinearVelocity(*m_bodyID).y); }
//	const Vector2D<float> PhysicsBody::GetCenterPosition() const { return Vector2D<float>(b2Body_GetPosition(*m_bodyID).x, b2Body_GetPosition(*m_bodyID).y); }
//	const Vector2D<float> PhysicsBody::GetTopLeftPosition() const { return Vector2D<float>(b2Body_GetPosition(*m_bodyID).x - m_width / 2.0f, b2Body_GetPosition(*m_bodyID).y - m_height / 2.0f); }
//
//	const float PhysicsBody::GetAngleInRadians() const 
//	{
//		b2Rot rotation = b2Body_GetRotation(*m_bodyID);
//		float angleInRadians = b2Rot_GetAngle(rotation);
//		return angleInRadians;
//	}
//	const float PhysicsBody::GetAngleInDegrees() const { return radiansToDegrees(GetAngleInRadians()); }
//}