#pragma once

#include "../../include/Physics/Box.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Physics/ConversionFunctions.h"
#include "../../include/Physics/BodyTypes.h"

namespace Engine
{
	void Box::bodyNotInWorldAlert() const { ENGINE_CRITICAL("Body not in world. Cannot get position."); }

	Box::Box(const BodyType bodyType, const float startingXInMeters, const float startingYInMeters,
		const float widthInMeters, const float heightInMeters, const float density, const float friction, const float angle,
		const float restitution, const float restitutionThreshold, bool visible, bool collidable, bool fixed)
		: m_halfWidthInMeters(widthInMeters / 2.0f), m_halfHeightInMeters(heightInMeters / 2.0f), 
		m_widthInMeters(widthInMeters), m_heightInMeters(heightInMeters),
		m_restitution(restitution), m_restitutionThreshold(restitutionThreshold),
		m_bodyType(bodyType), m_visible(visible), m_collidable(collidable), m_fixed(fixed), m_body(nullptr)
	{
		switch (bodyType)
		{
		case STATIC:
			m_bodyDef.type = b2_staticBody;
			m_bodyDef.fixedRotation = true;
			ENGINE_TRACE_D("Creating static body.");
			break;
		case DYNAMIC:
			m_bodyDef.type = b2_dynamicBody;
			// m_bodyDef.fixedRotation = false;
			m_bodyDef.fixedRotation = true;
			ENGINE_TRACE_D("Creating dynamic body.");
			break;
		default:
			m_bodyDef.type = b2_staticBody;
			m_bodyDef.fixedRotation = true;
			ENGINE_WARN_D("Invalid body type. Creating static body.");
			break;
		}

		m_prevX = startingXInMeters;
		m_prevY = startingYInMeters;
		m_bodyDef.position.Set(startingXInMeters, startingYInMeters);
		m_bodyDef.angle = degreesToRadians(angle);
		m_shape.SetAsBox(m_halfWidthInMeters, m_halfHeightInMeters);
		m_fixtureDef.density = density;
		m_fixtureDef.friction = friction;
		m_fixtureDef.shape = &m_shape;
		m_fixtureDef.restitution = restitution;
		m_fixtureDef.restitutionThreshold = restitutionThreshold;

		ENGINE_INFO_D("Box created at position: {0}, {1}. With width: {2}, height: {3}",
			startingXInMeters, startingYInMeters, m_widthInMeters, m_heightInMeters);
	}

	const BodyType Box::getBodyType() const { return m_bodyType; }

	const float Box::getWidthInMeters() const { return m_widthInMeters; }
	const float Box::getHeightInMeters() const { return m_heightInMeters; }

	void Box::createFixture() { m_body->CreateFixture(&m_fixtureDef); }

	const float Box::getPrevX() const { return m_prevX; }
	const float Box::getPrevY() const { return m_prevY; };
	void Box::updatePrevX(){ m_prevX = getTopLeftXInMeters(); };
	void Box::updatePrevY() { m_prevY = getTopLeftYInMeters(); };

	const float Box::getCenterXInMeters() const { return (m_body->GetPosition().x); }
	const float Box::getCenterYInMeters() const { return (m_body->GetPosition().y); }
	const float Box::getTopLeftXInMeters() const { return (m_body->GetPosition().x - m_widthInMeters / 2.0f); }
	const float Box::getTopLeftYInMeters() const { return (m_body->GetPosition().y - m_heightInMeters / 2.0f); }

	const float Box::getAngleInRadians() const { return m_body->GetAngle(); }
	const float Box::getAngleInDegrees() const { return radiansToDegrees(m_body->GetAngle()); }

	void Box::removeBodyFromWorld()
	{
		if (m_body != nullptr)
		{
			m_body->GetWorld()->DestroyBody(m_body);
			m_body = nullptr;
		}
	}

	void Box::SetXVelocity(const float xVel) { m_body->SetLinearVelocity(b2Vec2(xVel, m_body->GetLinearVelocity().y)); }
	void Box::SetYVelocity(const float yVel) { m_body->SetLinearVelocity(b2Vec2(m_body->GetLinearVelocity().x, yVel)); }

	void Box::SetGravity(bool enabled) { m_body->SetGravityScale(enabled ? 1.0f : 0.0f); }

	Box::~Box()
	{
		ENGINE_INFO_D("Freeing Box!");
		if (m_body != nullptr)
		{
			m_body->GetWorld()->DestroyBody(m_body);
			m_body = nullptr;
		}
	}
}