#pragma once

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
		
		bool m_collidable;
		bool m_fixed;
		bool m_gravityOn;

	public:
		Box() = default;
		Box(const BodyType bodyType, const bool fixed, const float startingXInMeters,
			const float startingYInMeters, const float widthInMeters, const float heightInMeters);

		~Box();

		// Box2D specific functions
		void RemoveBodyFromWorld() override;

		void ApplyForceToBox(std::pair<float, float> force) override;
		void ApplyImpulseToBox(std::pair<float, float> impulse) override;

		bool OnGround() const override;

		// Box2d getter and setter wrappers
		void CreateFixture() override;
		void SetGravity(bool enabled) override;
		void SetXDeceleration(const float xDecel) override;
		void SetXVelocity(const float xVel) override;
		void SetYVelocity(const float yVel) override;
		void SetFixedRotation(bool fixed) override;
		void SetDensity(const float density);
		void SetFriction(const float friction);
		void SetRestitution(const float restitution);
		void SetRestitutionThreshold(const float threshold);
		void SetCollidable(const bool collidable);
		const float GetXVelocity() const override;
		const float GetYVelocity() const override;
		const float GetCenterXInMeters() const override;
		const float GetCenterYInMeters() const override;
		const float GetTopLeftXInMeters() const override;
		const float GetTopLeftYInMeters() const override;
		const BodyType GetBodyType() const override;
		const float GetAngleInRadians() const override;
		const float GetAngleInDegrees() const override;
		
		// Box2D does not track previous values.
		// Need to update them here.
		void UpdatePrevPosition() override;

		// Non-box2d getters
		const float GetTopLeftPrevX() const override;
		const float GetTopLeftPrevY() const override;
		const float GetWidthInMeters() const override; 
		const float GetHeightInMeters() const override;

	};
}