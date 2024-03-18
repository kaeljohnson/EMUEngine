#pragma once

#include "../Logging/Logger.h"
#include "box2d/box2d.h"

namespace Engine
{
	// This class primarily serves as a wrapper for the Box2D b2Body class. It will be used by the 
	// engine as the object that adheres to physics and collission detection. This class
	// is not meant to be used by the client. It is not the "entity" or "game object" class. It will not handle
	// textures or animation. Those classes will inherit from this class.

	class Box
	{
	private:
		b2Body* m_body;
		b2BodyDef m_bodyDef;
		b2FixtureDef m_fixtureDef;
		b2PolygonShape m_shape;
		
		int m_bodyType;

		float m_halfWidth;
		float m_halfHeight;
		float m_width;
		float m_height;

	public:
		Box() = default;
		Box(int bodyType, float x, float y, float halfWidth, float halfHeight, float density, float friction);
		const b2Body* getBody() const;
		void setBody(b2Body* body);
		const b2BodyDef& getBodyDef() const;

		void createFixture();

		const int getCenterX() const;
		const int getCenterY() const;
		const int getTopLeftX() const;
		const int getTopLeftY() const;
		const int getWidth() const; 
		const int getHeight() const;
		const float getAngle() const;
	};
}