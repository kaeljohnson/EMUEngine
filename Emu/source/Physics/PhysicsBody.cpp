#pragma once

#include <utility>
#include "../../include/MathUtil.h"

#include "../../include/Physics/PhysicsBody.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Physics/BodyTypes.h"

namespace Engine
{
	PhysicsBody::PhysicsBody(const BodyType bodyType, const bool fixed, const Vector2D<float> position, const Vector2D<float> size)
		: m_halfWidth(size.X / 2.0f), m_halfHeight(size.Y / 2.0f), 
		m_width(size.X), m_height(size.Y),
		m_bodyType(bodyType), m_collidable(true), m_fixed(fixed), m_body(nullptr), m_fixture(nullptr),
		m_bottomCollision(false), m_topCollision(false), m_leftCollision(false), m_rightCollision(false),
		m_bottomSensor(false), m_topSensor(false), m_leftSensor(false), m_rightSensor(false),
		m_gravityOn(true)
	{
		switch (bodyType)
		{
		case STATIC:
			ENGINE_TRACE_D("Creating static body.");
			break;
		case DYNAMIC:
			m_bodyDef.type = b2_dynamicBody;
			ENGINE_TRACE_D("Creating dynamic body.");
			break;
		case KINEMATIC:
			m_bodyDef.type = b2_kinematicBody;
			ENGINE_TRACE_D("Creating kinematic body.");
			break;
		case SENSOR:
			m_bodyDef.type = b2_kinematicBody;
			ENGINE_TRACE_D("Creating sensor.");
			m_fixtureDef.isSensor = true;
			break;
		default:
			m_bodyDef.type = b2_staticBody;
			ENGINE_WARN_D("Invalid body type. Creating static body.");
			break;
		}

		m_bodyDef.fixedRotation = true;
		m_bodyDef.userData.pointer = reinterpret_cast<intptr_t>(this);
		m_prevPosition.X = position.X;
		m_prevPosition.Y = position.Y;
		m_bodyDef.position.Set(position.X + m_halfWidth, position.Y + m_halfHeight);
		m_shape.SetAsBox(m_halfWidth, m_halfHeight);
		m_fixtureDef.shape = &m_shape;
		m_fixtureDef.restitution = 0.0f;
		m_fixtureDef.restitutionThreshold = 0.0f;
		m_fixtureDef.density = 1.0f;
		m_fixtureDef.friction = 1.0f;

		ENGINE_INFO_D("Box created at position: " + std::to_string(position.X) + "," 
			+ std::to_string(position.Y) + ". With width: " + std::to_string(size.X) + ", height: " + std::to_string(size.Y));
	}

	PhysicsBody::~PhysicsBody()
	{
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

				b2Body* otherBody = edge->contact->GetFixtureA()->GetBody() == m_body ? edge->contact->GetFixtureB()->GetBody() : edge->contact->GetFixtureA()->GetBody();
				PhysicsBody* otherPhysicsBody = reinterpret_cast<PhysicsBody*>(otherBody->GetUserData().pointer);

				// Assuming the first point's normal is representative for the whole contact
				b2Vec2 normal = worldManifold.normal;

				// Different trigger for contacts for kinematic bodies for now.
				// Might need a custom body type for bodies that
				// are meant to be the ground.
				if (otherPhysicsBody->GetBodyType() == SENSOR)
				{
					if (normal.y < -0.5) // Collision from above `this`
					{
						this->SetBottomSensor(true);
						otherPhysicsBody->SetTopSensor(true);
					}
					else if (normal.y > 0.5) // Collision from below `this`
					{
						this->SetTopSensor(true);
						otherPhysicsBody->SetBottomSensor(true);
					}

					if (normal.x > 0.5) // Collision from the left of `this`
					{
						this->SetLeftSensor(true);
						otherPhysicsBody->SetRightSensor(true);
					}
					else if (normal.x < -0.5) // Collision from the right of `this`
					{
						this->SetRightSensor(true);
						otherPhysicsBody->SetLeftSensor(true);
					}
				}
				else
				{
					// Determine the direction of the collision for `this`

					if (normal.y < -0.5) // Collision from above `this`
					{
						this->SetBottomCollision(true);
						otherPhysicsBody->SetTopCollision(true);
					}
					else if (normal.y > 0.5) // Collision from below `this`
					{
						this->SetTopCollision(true);
						otherPhysicsBody->SetBottomCollision(true);
					}

					if (normal.x > 0.5) // Collision from the left of `this`
					{
						this->SetLeftCollision(true);
						otherPhysicsBody->SetRightCollision(true);
					}
					else if (normal.x < -0.5) // Collision from the right of `this`
					{
						this->SetRightCollision(true);
						otherPhysicsBody->SetLeftCollision(true);
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

	void PhysicsBody::CreateFixture() { m_fixture = m_body->CreateFixture(&m_fixtureDef); }
	void PhysicsBody::SetGravity(bool enabled) { m_body->SetGravityScale(enabled ? 1.0f : 0.0f); }

	void PhysicsBody::SetXVelocity(const float xVel) { m_body->SetLinearVelocity(b2Vec2(xVel, m_body->GetLinearVelocity().y)); }
	void PhysicsBody::SetYVelocity(const float yVel) { m_body->SetLinearVelocity(b2Vec2(m_body->GetLinearVelocity().x, yVel)); }
	void PhysicsBody::SetXDeceleration(const float xDecel) { m_body->SetLinearDamping(xDecel); }
	void PhysicsBody::SetFixedRotation(bool fixed) { m_body->SetFixedRotation(fixed); }
	void PhysicsBody::SetDensity(const float density) { m_fixtureDef.density = density; }
	void PhysicsBody::SetFriction(const float friction) { m_fixtureDef.friction = friction; }
	void PhysicsBody::SetRestitution(const float restitution) { m_fixtureDef.restitution = restitution; }
	void PhysicsBody::SetRestitutionThreshold(const float threshold) { m_fixtureDef.restitutionThreshold = threshold; }
	void PhysicsBody::SetCollidable(const bool collidable) { m_collidable = collidable; }
	void PhysicsBody::SetWidth(const float width) { m_shape.SetAsBox(width / 2.0f, m_halfHeight); }
	void PhysicsBody::SetHeight(const float height) { m_shape.SetAsBox(m_halfWidth, height / 2.0f); }

	void PhysicsBody::SetBottomCollision(const bool bottomCollision) { m_bottomCollision = bottomCollision; }
	void PhysicsBody::SetTopCollision(const bool topCollision) { m_topCollision = topCollision; }
	void PhysicsBody::SetLeftCollision(const bool leftCollision) { m_leftCollision = leftCollision; }
	void PhysicsBody::SetRightCollision(const bool rightCollision) { m_rightCollision = rightCollision; }

	void PhysicsBody::SetBottomSensor(const bool bottomSensor) { m_bottomSensor = bottomSensor; }
	void PhysicsBody::SetTopSensor(const bool topSensor) { m_topSensor = topSensor; }
	void PhysicsBody::SetLeftSensor(const bool leftSensor) { m_leftSensor = leftSensor; }
	void PhysicsBody::SetRightSensor(const bool rightSensor) { m_rightSensor = rightSensor; }
}