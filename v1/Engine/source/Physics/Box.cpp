#pragma once

#include "../../include/Physics/Box.h"

namespace Engine
{

	Box::Box(int bodyType, float startingX, float startingY, float halfWidth, float halfHeight, float density, float friction)
		: m_halfWidth(halfWidth), m_halfHeight(halfHeight), m_bodyType(bodyType), m_width(0), m_height(0)
	{
		switch (bodyType)
		{
		case 1:
			m_bodyDef.type = b2_staticBody;
			m_fixtureDef.density = 0;
			m_fixtureDef.friction = 0;
			break;
		case 2:
			m_bodyDef.type = b2_dynamicBody;
			m_fixtureDef.density = density;
			m_fixtureDef.friction = friction;
			break;
		default:
			m_bodyDef.type = b2_staticBody;
			m_fixtureDef.density = 0;
			m_fixtureDef.friction = 0;
			break;
		}

		m_bodyDef.position.Set(startingX, startingY);
		

		m_shape.SetAsBox(m_halfWidth, m_halfHeight);
		m_fixtureDef.shape = &m_shape;

		
		m_width = static_cast<int>((m_shape.m_vertices[1].x - m_shape.m_vertices[0].x) * 2);
		m_height = static_cast<int>((m_shape.m_vertices[2].y - m_shape.m_vertices[0].y) * 2);
		
	}

	const b2Body* Box::getBody() const { return m_body; }
	void Box::setBody(b2Body* body) { m_body = body; }
	void Box::createFixture() { m_body->CreateFixture(&m_fixtureDef); }

	const b2BodyDef& Box::getBodyDef() const { return m_bodyDef; }

	const int Box::getWidth() const { return m_width; }
	const int Box::getHeight() const { return m_height; }

	const int Box::getCenterX() const { return static_cast<int>(m_body->GetPosition().x); }
	const int Box::getCenterY() const { return static_cast<int>(m_body->GetPosition().y); }
	const int Box::getTopLeftX() const { return static_cast<int>(m_body->GetPosition().x - m_width / 2); }
	const int Box::getTopLeftY() const { return static_cast<int>(m_body->GetPosition().y + m_height / 2); }
	const float Box::getAngle() const { return m_body->GetAngle(); }

}