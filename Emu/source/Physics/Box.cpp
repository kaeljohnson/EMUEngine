#pragma once

#include <utility>

#include "../../include/Physics/Box.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Physics/ConversionFunctions.h"
#include "../../include/Physics/BodyTypes.h"

namespace Engine
{
	Box::Box(const BodyType bodyType, const bool fixed, const float startingXInMeters, const float startingYInMeters,
		const float widthInMeters, const float heightInMeters)
		: m_halfWidthInMeters(widthInMeters / 2.0f), m_halfHeightInMeters(heightInMeters / 2.0f), 
		m_widthInMeters(widthInMeters), m_heightInMeters(heightInMeters),
		m_bodyType(bodyType), m_collidable(true), m_fixed(fixed), m_body(nullptr),
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
		default:
			m_bodyDef.type = b2_staticBody;
			ENGINE_WARN_D("Invalid body type. Creating static body.");
			break;
		}

		m_bodyDef.fixedRotation = true;
		m_prevX = startingXInMeters;
		m_prevY = startingYInMeters;
		m_bodyDef.position.Set(startingXInMeters, startingYInMeters);
		m_shape.SetAsBox(m_halfWidthInMeters, m_halfHeightInMeters);
		m_fixtureDef.shape = &m_shape;
		m_fixtureDef.restitution = 0.0f;
		m_fixtureDef.restitutionThreshold = 0.0f;
		m_fixtureDef.density = 1.0f;
		m_fixtureDef.friction = 1.0f;

		ENGINE_INFO_D("Box created at position: {0}, {1}. With width: {2}, height: {3}",
			startingXInMeters, startingYInMeters, m_widthInMeters, m_heightInMeters);
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

	bool Box::OnGround() const
	{
		b2ContactEdge* edge = m_body->GetContactList();
		while (edge)
		{
			if (edge->contact->IsTouching())
			{
				b2WorldManifold worldManifold;
				edge->contact->GetWorldManifold(&worldManifold);

				// Check which body is the player body
				bool bodyAIsPlayer = edge->contact->GetFixtureA()->GetBody() == m_body;
				float normalY = bodyAIsPlayer ? worldManifold.normal.y : -worldManifold.normal.y;

				if (normalY > 0.5f)
				{
					return true;
				}
			}
			edge = edge->next;
		}
		return false;
	}

	void Box::UpdatePrevPosition() { m_prevX = GetTopLeftXInMeters(); m_prevY = GetTopLeftYInMeters(); }

	void Box::CreateFixture() { m_body->CreateFixture(&m_fixtureDef); }
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

	const BodyType Box::GetBodyType() const { return m_bodyType; }
	const float Box::GetWidthInMeters() const { return m_widthInMeters; }
	const float Box::GetHeightInMeters() const { return m_heightInMeters; }
	const float Box::GetSizeInMeters() const { return m_widthInMeters * m_heightInMeters; }
	const float Box::GetXVelocity() const { return m_body->GetLinearVelocity().x; }
	const float Box::GetYVelocity() const { return m_body->GetLinearVelocity().y; }
	
	const float Box::GetTopLeftPrevX() const { return m_prevX; }
	const float Box::GetTopLeftPrevY() const { return m_prevY; }

	const float Box::GetCenterXInMeters() const { return (m_body->GetPosition().x); }
	const float Box::GetCenterYInMeters() const { return (m_body->GetPosition().y); }
	const float Box::GetTopLeftXInMeters() const { return (m_body->GetPosition().x - m_widthInMeters / 2.0f); }
	const float Box::GetTopLeftYInMeters() const { return (m_body->GetPosition().y - m_heightInMeters / 2.0f); }

	const float Box::GetAngleInRadians() const { return m_body->GetAngle(); }
	const float Box::GetAngleInDegrees() const { return radiansToDegrees(m_body->GetAngle()); }
}