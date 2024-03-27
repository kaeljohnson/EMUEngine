#pragma once

#include "../Logging/Logger.h"
#include "box2d/box2d.h"
#include "ConversionFunctions.h"
#include "BodyTypes.h"

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
		
		const BodyType m_bodyType;

		const float m_halfWidthInMeters;
		const float m_halfHeightInMeters;
		const float m_widthInMeters;
		const float m_heightInMeters;

	public:
		Box() = default;
		Box(const BodyType bodyType, const float startingXInMeters, const float startingYInMeters, 
			const float halfWidthInMeters, const float halfHeightInMeters, const float density, const float friction);

		b2Body* getBody() const;
		void setBody(b2Body* body);
		const b2BodyDef& getBodyDef() const;

		void createFixture();

		const float getCenterXInMeters() const;
		const float getCenterYInMeters() const;
		const float getTopLeftXInMeters() const;
		const float getTopLeftYInMeters() const;
		const float getWidthInMeters() const; 
		const float getHeightInMeters() const;

		const int getCenterXInPixels() const;
		const int getCenterYInPixels() const;
		const int getTopLeftXInPixels() const;
		const int getTopLeftYInPixels() const;
		const int getWidthInPixels() const;
		const int getHeightInPixels() const;

		const float getAngleInRadians() const;
		const double getAngleInDegrees() const;

		const BodyType getBodyType() const;
		void bodyNotInWorldAlert() const;
	};
}