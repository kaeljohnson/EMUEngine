#pragma once

#include "../../include/Physics/Box.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Physics/ConversionFunctions.h"
#include "../../include/Physics/BodyTypes.h"

namespace Engine
{
	void Box::bodyNotInWorldAlert() const { ENGINE_CRITICAL("Body not in world. Cannot get position."); }

	Box::Box(const BodyType bodyType, const float startingXInMeters, const float startingYInMeters, 
		const float halfWidthInMeters, const float halfHeightInMeters, const float density, const float friction)
		: m_halfWidthInMeters(halfWidthInMeters), m_halfHeightInMeters(halfHeightInMeters), 
		m_widthInMeters(m_halfWidthInMeters * 2.0f), m_heightInMeters(m_halfHeightInMeters * 2.0f),
		m_bodyType(bodyType), m_body(nullptr)
	{
		switch (bodyType)
		{
		case STATIC:
			m_bodyDef.type = b2_staticBody;
			m_fixtureDef.density = 0;
			m_fixtureDef.friction = 0;
			m_bodyDef.fixedRotation = true;
			ENGINE_TRACE("Creating static body.");
			break;
		case DYNAMIC:
			m_bodyDef.type = b2_dynamicBody;
			m_fixtureDef.density = density;
			m_fixtureDef.friction = friction;
			m_bodyDef.fixedRotation = false;
			ENGINE_TRACE("Creating dynamic body.");
			break;
		default:
			m_bodyDef.type = b2_staticBody;
			m_fixtureDef.density = 0;
			m_fixtureDef.friction = 0;
			m_bodyDef.fixedRotation = true;
			ENGINE_WARN("Invalid body type. Creating static body.");
			break;
		}

		m_bodyDef.position.Set(startingXInMeters, startingYInMeters);
		m_shape.SetAsBox(m_halfWidthInMeters, m_halfHeightInMeters);
		m_fixtureDef.shape = &m_shape;

		ENGINE_INFO("Box created at position: {0}, {1}. With width: {2}, height: {3}", 
			startingXInMeters, startingYInMeters, m_widthInMeters, m_heightInMeters);
	}

	b2Body* Box::getBody() const { return m_body; }
	const b2BodyDef& Box::getBodyDef() const { return m_bodyDef; }

	const int Box::getWidthInMeters() const { return static_cast<int>(m_widthInMeters); }
	const int Box::getHeightInMeters() const { return static_cast<int>(m_heightInMeters); }

	const int Box::getWidthInPixels() const { return static_cast<int>(m_widthInMeters); }
	const int Box::getHeightInPixels() const { return static_cast<int>(m_heightInMeters); }

	void Box::setBody(b2Body* body) { m_body = body; }
	void Box::createFixture() { m_body->CreateFixture(&m_fixtureDef); }

	const double Box::getCenterXInMeters() const { return (m_body->GetPosition().x); }
	const double Box::getCenterYInMeters() const { return (m_body->GetPosition().y); }
	const double Box::getTopLeftXInMeters() const { return (m_body->GetPosition().x - m_widthInMeters / 2.0); }
	const double Box::getTopLeftYInMeters() const { return (m_body->GetPosition().y - m_heightInMeters / 2.0); }

	const int Box::getCenterXInPixels() const { return static_cast<int>(round(m_body->GetPosition().x)); }
	const int Box::getCenterYInPixels() const { return static_cast<int>(round(m_body->GetPosition().y)); }
	const int Box::getTopLeftXInPixels() const { return static_cast<int>(round(m_body->GetPosition().x - m_widthInMeters / 2.0f)); }
	const int Box::getTopLeftYInPixels() const { return static_cast<int>(round(m_body->GetPosition().y - m_heightInMeters / 2.0f)); }

	const float Box::getAngleInRadians() const { return m_body->GetAngle(); }
	const double Box::getAngleInDegrees() const { return radiansToDegrees(m_body->GetAngle()); }

	const BodyType Box::getBodyType() const { return m_bodyType; }
}