#pragma once

#include <utility>

#include "../../include/Physics/Box.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Physics/BodyTypes.h"

namespace Engine
{
	Box::Box(const BodyType bodyType, const bool fixed, const float startingXInMeters, const float startingYInMeters,
		const float widthInMeters, const float heightInMeters)
		: m_halfWidthInMeters(widthInMeters / 2.0f), m_halfHeightInMeters(heightInMeters / 2.0f), 
		m_widthInMeters(widthInMeters), m_heightInMeters(heightInMeters),
		m_bodyType(bodyType), m_collidable(true), m_fixed(fixed), m_body(nullptr),
		m_gravityOn(true), OnGroundFlag(false), IsCollidingWith(false), IsBeingCollidedWith(false)
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
			m_fixtureDef.isSensor = true;
			break;
		default:
			m_bodyDef.type = b2_staticBody;
			ENGINE_WARN_D("Invalid body type. Creating static body.");
			break;
		}

		m_bodyDef.fixedRotation = true;
		m_bodyDef.userData.pointer = reinterpret_cast<intptr_t>(this);
		m_prevX = startingXInMeters;
		m_prevY = startingYInMeters;
		m_bodyDef.position.Set(startingXInMeters + m_halfWidthInMeters, startingYInMeters + m_halfHeightInMeters);
		m_shape.SetAsBox(m_halfWidthInMeters, m_halfHeightInMeters);
		m_fixtureDef.shape = &m_shape;
		m_fixtureDef.restitution = 0.0f;
		m_fixtureDef.restitutionThreshold = 0.0f;
		m_fixtureDef.density = 1.0f;
		m_fixtureDef.friction = 1.0f;

		ENGINE_INFO_D("Box created at position: " + std::to_string(startingXInMeters) + "," 
			+ std::to_string(startingYInMeters) + ". With width: " + std::to_string(m_widthInMeters) + ", height: " + std::to_string(m_heightInMeters));
	}

	Box::~Box()
	{
		ENGINE_INFO_D("Freeing Box!");
		if (m_body != nullptr)
		{
			m_body->GetWorld()->DestroyBody(m_body);
			m_body = nullptr;
		}
	}

	void Box::RemoveBodyFromWorld()
	{
		if (m_body != nullptr)
		{
			m_body->GetWorld()->DestroyBody(m_body);
			m_body = nullptr;
		}
	}

	void Box::ApplyForceToBox(std::pair<float, float> force)
	{
		m_body->ApplyForceToCenter(b2Vec2(force.first, force.second), true);
	}

	void Box::ApplyImpulseToBox(std::pair<float, float> impulse)
	{
		m_body->ApplyLinearImpulseToCenter(b2Vec2(impulse.first, impulse.second), true);
	}

	void Box::SetCollisionFlags()
	{
		SetBottomCollision(false);
		SetTopCollision(false);
		SetLeftCollision(false);
		SetRightCollision(false);

		SetBottomSensor(false);
		SetTopSensor(false);
		SetLeftSensor(false);
		SetRightSensor(false);

		b2ContactEdge* edge = m_body->GetContactList();
		while (edge)
		{
			if (edge->contact->IsTouching())
			{
				b2WorldManifold worldManifold;
				edge->contact->GetWorldManifold(&worldManifold);

				b2Body* otherBody = edge->contact->GetFixtureA()->GetBody() == m_body ? edge->contact->GetFixtureB()->GetBody() : edge->contact->GetFixtureA()->GetBody();
				Box* otherBox = reinterpret_cast<Box*>(otherBody->GetUserData().pointer);

				// Assuming the first point's normal is representative for the whole contact
				b2Vec2 normal = worldManifold.normal;

				// Different trigger for contacts for kinematic bodies for now.
				// Might need a custom body type for bodies that
				// are meant to be the ground.
				if (otherBody->GetType() == b2_kinematicBody)
				{
					if (normal.y < -0.5) // Collision from above `this`
					{
						this->SetBottomSensor(true);
						otherBox->SetTopSensor(true);
					}
					else if (normal.y > 0.5) // Collision from below `this`
					{
						this->SetTopSensor(true);
						otherBox->SetBottomSensor(true);
					}

					if (normal.x > 0.5) // Collision from the left of `this`
					{
						this->SetLeftSensor(true);
						otherBox->SetRightSensor(true);
					}
					else if (normal.x < -0.5) // Collision from the right of `this`
					{
						this->SetRightSensor(true);
						otherBox->SetLeftSensor(true);
					}
				}
				else
				{
					// Determine the direction of the collision for `this`

					if (normal.y < -0.5) // Collision from above `this`
					{
						this->SetBottomCollision(true);
						otherBox->SetTopCollision(true);
					}
					else if (normal.y > 0.5) // Collision from below `this`
					{
						this->SetTopCollision(true);
						otherBox->SetBottomCollision(true);
					}

					if (normal.x > 0.5) // Collision from the left of `this`
					{
						this->SetLeftCollision(true);
						otherBox->SetRightCollision(true);
					}
					else if (normal.x < -0.5) // Collision from the right of `this`
					{
						this->SetRightCollision(true);
						otherBox->SetLeftCollision(true);
					}
				}
			}
			edge = edge->next;
		}
	}

	void Box::UpdatePrevPosition() { m_prevX = GetTopLeftXInMeters(); m_prevY = GetTopLeftYInMeters(); }

	void Box::CreateFixture() { m_fixture = m_body->CreateFixture(&m_fixtureDef); }
	void Box::SetGravity(bool enabled) { m_body->SetGravityScale(enabled ? 1.0f : 0.0f); }

	void Box::SetXVelocity(const float xVel) { m_body->SetLinearVelocity(b2Vec2(xVel, m_body->GetLinearVelocity().y)); }
	void Box::SetYVelocity(const float yVel) { m_body->SetLinearVelocity(b2Vec2(m_body->GetLinearVelocity().x, yVel)); }
	void Box::SetXDeceleration(const float xDecel) { m_body->SetLinearDamping(xDecel); }
	void Box::SetFixedRotation(bool fixed) { m_body->SetFixedRotation(fixed); }
	void Box::SetDensity(const float density) { m_fixtureDef.density = density; }
	void Box::SetFriction(const float friction) { m_fixtureDef.friction = friction; }
	void Box::SetRestitution(const float restitution) { m_fixtureDef.restitution = restitution; }
	void Box::SetRestitutionThreshold(const float threshold) { m_fixtureDef.restitutionThreshold = threshold; }
	void Box::SetCollidable(const bool collidable) { m_collidable = collidable; }
	void Box::SetWidthInMeters(const float widthInMeters) { m_shape.SetAsBox(widthInMeters / 2.0f, m_halfHeightInMeters); }
	void Box::SetHeightInMeters(const float heightInMeters) { m_shape.SetAsBox(m_halfWidthInMeters, heightInMeters / 2.0f); }

	void Box::SetBottomCollision(const bool bottomCollision) { BottomCollision = bottomCollision; }
	void Box::SetTopCollision(const bool topCollision) { TopCollision = topCollision; }
	void Box::SetLeftCollision(const bool leftCollision) { LeftCollision = leftCollision; }
	void Box::SetRightCollision(const bool rightCollision) { RightCollision = rightCollision; }

	void Box::SetBottomSensor(const bool bottomSensor) { BottomSensor = bottomSensor; }
	void Box::SetTopSensor(const bool topSensor) { TopSensor = topSensor; }
	void Box::SetLeftSensor(const bool leftSensor) { LeftSensor = leftSensor; }
	void Box::SetRightSensor(const bool rightSensor) { RightSensor = rightSensor; }
}