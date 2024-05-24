#pragma once

#include <utility>

#include "../Core.h"

#include "box2d/box2d.h"

#include "BodyTypes.h"
#include "IPhysicsBody.h"

namespace Engine
{
	// This class primarily serves as a wrapper for the Box2D b2Body class. It will be used by the 
	// engine as the object that adheres to physics and collission detection. This class
	// is not meant to be used by the client. It is not the "entity" or "game object" class. It will not handle
	// textures or animation.

	class Box : public IPhysicsBody
	{
	public:

		// Box2D specific members.
		b2Body* m_body;
		b2BodyDef m_bodyDef;
		b2FixtureDef m_fixtureDef;
		b2PolygonShape m_shape;
		
		const BodyType m_bodyType;

		// State members
		const float m_halfWidthInMeters;
		const float m_halfHeightInMeters;
		const float m_widthInMeters;
		const float m_heightInMeters;

		float m_prevX;
		float m_prevY;

		float m_restitution;
		float m_restitutionThreshold;

		bool m_visible;
		bool m_collidable;
		bool m_fixed;

	public:
		Box() = default;
		Box(const BodyType bodyType, const float startingXInMeters, const float startingYInMeters,
			const float widthInMeters, const float heightInMeters, const float density, const float friction, const float angle,
			const float restitution, const float restitutionThreshold, bool visible, bool collidable, bool fixed);

		~Box();

		// Calls Box2D CreateFixture.
		void createFixture() override;

		// Getters
		const float getPrevX() const override;
		const float getPrevY() const override;
		
		const float getCenterXInMeters() const override;
		const float getCenterYInMeters() const override;
		const float getTopLeftXInMeters() const override;
		const float getTopLeftYInMeters() const override;

		const float getWidthInMeters() const override; 
		const float getHeightInMeters() const override;

		const float getAngleInRadians() const override;
		const float getAngleInDegrees() const override;

		const BodyType getBodyType() const override;

		// Box2D does not track previous values. 
		// Need to update them here.
		void updatePrevX() override;
		void updatePrevY() override;

		void bodyNotInWorldAlert() const;
		void removeBodyFromWorld() override;

		void SetXDeceleration(const float xDecel) override;

		void SetXVelocity(const float xVel) override;
		void SetYVelocity(const float yVel) override;
		const float GetXVelocity() const override;
		const float GetYVelocity() const override;

		void SetGravity(bool enabled) override;

		void ApplyForceToBox(std::pair<float, float> force) override;
		void ApplyImpulseToBox(std::pair<float, float> impulse) override;
	};
}