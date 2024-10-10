#pragma once

#include <utility>
#include "../../include/MathUtil.h"

#include "../../include/Physics/PhysicsBody.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Physics/BodyTypes.h"
#include "../../include/ComponentManager/ComponentManager.h"

#include "box2d/box2d.h"

namespace Engine
{
	PhysicsBody::PhysicsBody(const size_t id) : m_halfWidth(0.0f), m_halfHeight(0.0f), m_width(0.0f), m_height(0.0f),
		m_startingPosition(Vector2D<float>(0.0f, 0.0f)), m_bodyType(STATIC), m_fixed(false), m_body(nullptr),
		m_bottomCollision(false), m_topCollision(false), m_leftCollision(false), m_rightCollision(false),
		m_bottomSensor(false), m_topSensor(false), m_leftSensor(false), m_rightSensor(false),
		m_gravityOn(true), m_isSensor(false), m_prevPosition(Vector2D<float>(0.0f, 0.0f)), m_fixedRotation(true), m_restitution(0.0f),
		m_restitutionThreshold(0.0f), m_density(1.0f), m_friction(0.0f), Component(id)
	
	{
	}

	PhysicsBody::PhysicsBody(const size_t id, const BodyType bodyType, const bool fixed, const Vector2D<float> position, const Vector2D<float> size)
		: m_halfWidth(size.X / 2.0f), m_halfHeight(size.Y / 2.0f), 
		m_width(size.X), m_height(size.Y), m_startingPosition(position),
		m_bodyType(bodyType), m_fixed(fixed), m_body(nullptr),
		m_bottomCollision(false), m_topCollision(false), m_leftCollision(false), m_rightCollision(false),
		m_bottomSensor(false), m_topSensor(false), m_leftSensor(false), m_rightSensor(false),
		m_gravityOn(true), m_isSensor(false), m_prevPosition(position), m_fixedRotation(true), m_restitution(0.0f),
		m_restitutionThreshold(0.0f), m_density(1.0f), m_friction(0.0f),
		Component(id)
	{}

	PhysicsBody::~PhysicsBody()
	{
		ENGINE_INFO_D("PhysicsBody destroyed with ID: " + std::to_string(m_id));
		RemoveBodyFromWorld();
	}

	void PhysicsBody::RemoveBodyFromWorld()
	{
		if (m_body != nullptr)
		{
			m_body->GetWorld()->DestroyBody(m_body);
			m_body = nullptr;
		}
	}

	void PhysicsBody::ApplyForceToBody(Vector2D<float> force)
	{
		m_body->ApplyForceToCenter(b2Vec2(force.X, force.Y), true);
	}

	void PhysicsBody::ApplyImpulseToBody(Vector2D<float> impulse)
	{
		m_body->ApplyLinearImpulseToCenter(b2Vec2(impulse.X, impulse.Y), true);
	}

	void PhysicsBody::SetStartingDensity(const float density)
	{
		m_density = density;
	}

	void PhysicsBody::SetDensity(const float density)
	{
		b2Fixture* fixture = m_body->GetFixtureList();
		if (fixture != nullptr)
			fixture->SetDensity(density);
		else
			ENGINE_CRITICAL_D("No fixture found for body!");
	}

	void PhysicsBody::SetStartingRestitution(const float restitution)
	{
		m_restitution = restitution;
	}

	void PhysicsBody::SetRestitution(const float restitution)
	{
		b2Fixture* fixture = m_body->GetFixtureList();
		if (fixture != nullptr)
			fixture->SetRestitution(restitution);
		else
			ENGINE_CRITICAL_D("No fixture found for body!");
	}

	void PhysicsBody::SetStartingFriction(const float friction)
	{
		m_friction = friction;
	}

	void PhysicsBody::SetFriction(const float friction)
	{
		b2Fixture* fixture = m_body->GetFixtureList();
		if (fixture != nullptr)
			fixture->SetFriction(friction);
		else
			ENGINE_CRITICAL_D("No fixture found for body!");
	}

	void PhysicsBody::SetIsSensor(const bool isSensor)
	{
		m_isSensor = isSensor;
	}

	void PhysicsBody::SetContactFlags()
	{
		m_bottomCollision = false;
		m_topCollision = false;
		m_leftCollision = false;
		m_rightCollision = false;

		m_bottomSensor = false;
		m_topSensor = false;
		m_leftSensor = false;
		m_rightSensor = false;

		b2ContactEdge* edge = m_body->GetContactList();
		while (edge)
		{
			if (edge->contact->IsTouching())
			{
				b2WorldManifold worldManifold;
				edge->contact->GetWorldManifold(&worldManifold);

				b2Body* otherBody;
				int directionFactor = 1;
				if (edge->contact->GetFixtureA()->GetBody() == m_body)
				{
					otherBody = edge->contact->GetFixtureB()->GetBody();
				}
				else
				{
					directionFactor = -1;
					otherBody = edge->contact->GetFixtureA()->GetBody();
				}
								
				PhysicsBody* otherPhysicsBody = reinterpret_cast<PhysicsBody*>(otherBody->GetUserData().pointer);

				// Assuming the first point's normal is representative for the whole contact
				b2Vec2 normal = worldManifold.normal;

				normal.x *= directionFactor;
				normal.y *= directionFactor;

				// Different trigger for contacts for kinematic bodies for now.
				// Might need a custom body type for bodies that
				// are meant to be the ground.
				if (otherPhysicsBody->GetBodyType() == SENSOR)
				{
					if (normal.y < -0.5) // Collision from above `this`
					{
						this->SetTopSensor(true);
						otherPhysicsBody->SetBottomSensor(true);
					}
					else if (normal.y > 0.5) // Collision from below `this`
					{
						this->SetBottomSensor(true);
						otherPhysicsBody->SetTopSensor(true);
					}

					if (normal.x > 0.5) // Collision from the Right of `this`
					{
						this->SetRightSensor(true);
						otherPhysicsBody->SetLeftSensor(true);
					}
					else if (normal.x < -0.5) // Collision from the Left of `this`
					{
						this->SetLeftSensor(true);
						otherPhysicsBody->SetRightSensor(true);
					}
				}
				else
				{
					// Determine the direction of the collision for `this`

					if (normal.y < -0.5) // Collision from above `this`
					{
						this->SetTopCollision(true);
						otherPhysicsBody->SetBottomCollision(true);
					}
					else if (normal.y > 0.5) // Collision from below `this`
					{
						this->SetBottomCollision(true);
						otherPhysicsBody->SetTopCollision(true);
					}

					if (normal.x > 0.5) // Collision from the right of `this`
					{
						this->SetRightCollision(true);
						otherPhysicsBody->SetLeftCollision(true);
					}
					else if (normal.x < -0.5) // Collision from the left of `this` 
					{
						this->SetLeftCollision(true);
						otherPhysicsBody->SetRightCollision(true);
					}
				}
			}
			edge = edge->next;
		}
	}

	void PhysicsBody::SetContactFlagsToFalse()
	{
		m_bottomCollision = false;
		m_topCollision = false;
		m_leftCollision = false;
		m_rightCollision = false;

		m_bottomSensor = false;
		m_topSensor = false;
		m_leftSensor = false;
		m_rightSensor = false;
	}

	void PhysicsBody::UpdatePrevPosition() { m_prevPosition = GetTopLeftPosition(); }
	void PhysicsBody::Update()
	{
		if (!m_isActive) return;

		switch (GetBodyType())
		{
		case DYNAMIC:
			UpdatePrevPosition();
			SetContactFlags();
			break;
		case STATIC:
			SetContactFlagsToFalse();
			break;
		case SENSOR:
			SetContactFlagsToFalse();
			break;
		default:
			ENGINE_WARN_D("Body type has no engine side updating.");
			break;
		}
	}

	void PhysicsBody::SetGravity(bool enabled) { m_body->SetGravityScale(enabled ? 1.0f : 0.0f); }

	void PhysicsBody::SetXVelocity(const float xVel) { m_body->SetLinearVelocity(b2Vec2(xVel, m_body->GetLinearVelocity().y)); }
	void PhysicsBody::SetYVelocity(const float yVel) { m_body->SetLinearVelocity(b2Vec2(m_body->GetLinearVelocity().x, yVel)); }
	void PhysicsBody::SetXDeceleration(const float xDecel) { m_body->SetLinearDamping(xDecel); }
	void PhysicsBody::SetFixedRotation(bool fixed) { m_body->SetFixedRotation(fixed); }

	void PhysicsBody::SetBottomCollision(const bool bottomCollision) { m_bottomCollision = bottomCollision; }
	void PhysicsBody::SetTopCollision(const bool topCollision) { m_topCollision = topCollision; }
	void PhysicsBody::SetLeftCollision(const bool leftCollision) { m_leftCollision = leftCollision; }
	void PhysicsBody::SetRightCollision(const bool rightCollision) { m_rightCollision = rightCollision; }

	void PhysicsBody::SetBottomSensor(const bool bottomSensor) { m_bottomSensor = bottomSensor; }
	void PhysicsBody::SetTopSensor(const bool topSensor) { m_topSensor = topSensor; }
	void PhysicsBody::SetLeftSensor(const bool leftSensor) { m_leftSensor = leftSensor; }
	void PhysicsBody::SetRightSensor(const bool rightSensor) { m_rightSensor = rightSensor; }

	const Vector2D<float> PhysicsBody::GetVelocity() const { return Vector2D<float>(m_body->GetLinearVelocity().x, m_body->GetLinearVelocity().y); }
	const Vector2D<float> PhysicsBody::GetCenterPosition() const { return Vector2D<float>(m_body->GetPosition().x, m_body->GetPosition().y); }
	const Vector2D<float> PhysicsBody::GetTopLeftPosition() const { return Vector2D<float>(m_body->GetPosition().x - m_width / 2.0f, m_body->GetPosition().y - m_height / 2.0f); }

	const float PhysicsBody::GetAngleInRadians() const { return m_body->GetAngle(); }
	const float PhysicsBody::GetAngleInDegrees() const { return radiansToDegrees(m_body->GetAngle()); }
}