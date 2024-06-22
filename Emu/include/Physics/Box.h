#pragma once

#include "../Core.h"

#include "ConversionFunctions.h"

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
		b2Fixture* m_fixture;
		
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

		bool OnGroundFlag;

		bool IsCollidingWith;
		bool IsBeingCollidedWith;

		bool BottomCollision;
		bool TopCollision;
		bool LeftCollision;
		bool RightCollision;

		bool BottomSensor;
		bool TopSensor;
		bool LeftSensor;
		bool RightSensor;

	public:
		Box() = default;
		Box(const BodyType bodyType, const bool fixed, const float startingXInMeters,
			const float startingYInMeters, const float widthInMeters, const float heightInMeters);

		~Box();

		// Box2D specific functions
		void RemoveBodyFromWorld() override;

		void ApplyForceToBox(std::pair<float, float> force) override;
		void ApplyImpulseToBox(std::pair<float, float> impulse) override;

		// Box2d getter and setter wrappers
		void CreateFixture() override;
		void SetGravity(bool enabled) override;
		void SetXDeceleration(const float xDecel) override;
		void SetXVelocity(const float xVel) override;
		void SetYVelocity(const float yVel) override;
		void SetFixedRotation(bool fixed) override;
		void SetDensity(const float density) override;
		void SetFriction(const float friction) override;
		void SetRestitution(const float restitution) override;
		void SetRestitutionThreshold(const float threshold) override;
		void SetCollidable(const bool collidable) override;
		void SetWidthInMeters(const float widthInMeters) override;
		void SetHeightInMeters(const float heightInMeters) override;

		void SetCollisionFlags() override;

		void SetBottomCollision(const bool bottomCollision) override;
		void SetTopCollision(const bool topCollision) override;
		void SetLeftCollision(const bool leftCollision) override;
		void SetRightCollision(const bool rightCollision) override;

		void SetBottomSensor(const bool bottomSensor) override;
		void SetTopSensor(const bool topSensor) override;
		void SetLeftSensor(const bool leftSensor) override;
		void SetRightSensor(const bool rightSensor) override;

		inline const bool GetHasBottomCollision() const { return BottomCollision; }
		inline const bool GetHasTopCollision() const { return TopCollision; }
		inline const bool GetHasLeftCollision() const { return LeftCollision; }
		inline const bool GetHasRightCollision() const { return RightCollision; }

		inline const bool GetHasBottomSensor() const { return BottomSensor; }
		inline const bool GetHasTopSensor() const { return TopSensor; }
		inline const bool GetHasLeftSensor() const { return LeftSensor; }
		inline const bool GetHasRightSensor() const { return RightSensor; }

		inline const float GetXVelocity() const override { return m_body->GetLinearVelocity().x; }
		inline const float GetYVelocity() const override { return m_body->GetLinearVelocity().y; }
		inline const float GetCenterXInMeters() const override { return m_body->GetPosition().x; }
		inline const float GetCenterYInMeters() const override { return m_body->GetPosition().y; }
		inline const float GetTopLeftXInMeters() const override { return (m_body->GetPosition().x - m_widthInMeters / 2.0f); }
		inline const float GetTopLeftYInMeters() const override { return (m_body->GetPosition().y - m_heightInMeters / 2.0f); }
		inline const BodyType GetBodyType() const override { return m_bodyType; }
		inline const float GetAngleInRadians() const override { return m_body->GetAngle(); }
		inline const float GetAngleInDegrees() const override { return radiansToDegrees(m_body->GetAngle()); }
		
		// Box2D does not track previous values.
		// Need to update them here.
		void UpdatePrevPosition() override;

		// Non-box2d getters
		inline const float GetTopLeftPrevX() const override { return m_prevX; }
		inline const float GetTopLeftPrevY() const override { return m_prevY; }
		inline const float GetWidthInMeters() const override { return m_widthInMeters; }
		inline const float GetHeightInMeters() const override { return m_heightInMeters; }
		inline const float GetSizeInMeters() const override { return m_widthInMeters * m_heightInMeters; }
	};
}