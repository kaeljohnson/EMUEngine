#pragma once

#include "../Core.h"
#include "../MathUtil.h"

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

	class PhysicsBody : public IPhysicsBody
	{
	public:
		const BodyType m_bodyType;

		// Box2D specific members.
		b2Body* m_body;
		b2BodyDef m_bodyDef;
		b2FixtureDef m_fixtureDef;
		b2PolygonShape m_shape;
		b2Fixture* m_fixture;

		// State members
		const float m_halfWidth;
		const float m_halfHeight;
		const float m_width;
		const float m_height;

		// float m_prevX;
		// float m_prevY;
		Vector2D<float> m_prevPosition;
		
		bool m_collidable;
		bool m_fixed;
		bool m_gravityOn;


		bool m_bottomCollision;
		bool m_topCollision;
		bool m_leftCollision;
		bool m_rightCollision;

		bool m_bottomSensor;
		bool m_topSensor;
		bool m_leftSensor;
		bool m_rightSensor;

	public:
		PhysicsBody() = default;
		PhysicsBody(const BodyType bodyType, const bool fixed, const Vector2D<float> position, const Vector2D<float> size);

		~PhysicsBody();

		// PhysicsBody2D specific functions
		void RemoveBodyFromWorld() override;

		void ApplyForceToBody(Vector2D<float> force) override;
		void ApplyImpulseToBody(Vector2D<float> impulse) override;

		// PhysicsBody2d getter and setter wrappers
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
		void SetWidth(const float width) override;
		void SetHeight(const float height) override;

		void SetContactFlags() override;
		void SetContactFlagsToFalse() override;

		void SetBottomCollision(const bool bottomCollision) override;
		void SetTopCollision(const bool topCollision) override;
		void SetLeftCollision(const bool leftCollision) override;
		void SetRightCollision(const bool rightCollision) override;

		void SetBottomSensor(const bool bottomSensor) override;
		void SetTopSensor(const bool topSensor) override;
		void SetLeftSensor(const bool leftSensor) override;
		void SetRightSensor(const bool rightSensor) override;

		inline const bool GetHasCollisionBelow() const { return m_bottomCollision; }
		inline const bool GetHasCollisionAbove() const { return m_topCollision; }
		inline const bool GetHasCollisionLeft() const { return m_leftCollision; }
		inline const bool GetHasCollisionRight() const { return m_rightCollision; }

		inline const bool GetHasSensorBelow() const { return m_bottomSensor; }
		inline const bool GetHasSensorAbove() const { return m_topSensor; }
		inline const bool GetHasSensorLeft() const { return m_leftSensor; }
		inline const bool GetHasSensorRight() const { return m_rightSensor; }

		inline const Vector2D<float> GetVelocity() const override { return Vector2D<float>(m_body->GetLinearVelocity().x, m_body->GetLinearVelocity().y); }
		inline const Vector2D<float> GetCenterPosition() const override { return Vector2D<float>(m_body->GetPosition().x, m_body->GetPosition().y); }
		inline const Vector2D<float> GetTopLeftPosition() const override { return Vector2D<float>(m_body->GetPosition().x - m_width / 2.0f, m_body->GetPosition().y - m_height / 2.0f); }

		inline const BodyType GetBodyType() const override { return m_bodyType; }

		inline const float GetAngleInRadians() const override { return m_body->GetAngle(); }
		inline const float GetAngleInDegrees() const override { return radiansToDegrees(m_body->GetAngle()); }
		
		// PhysicsBody2D does not track previous values.
		// Need to update them here.
		void UpdatePrevPosition() override;

		// Non-PhysicsBody2d getters
		inline const Vector2D<float> GetTopLeftPrevPosition() const override { return m_prevPosition; }
		inline const Vector2D<float> GetCenterPrevPosition() const override { return Vector2D<float>(m_prevPosition.X + m_halfWidth, m_prevPosition.Y + m_halfHeight); }

		inline const Vector2D<float> GetDimensions() const override { return Vector2D<float>(m_width, m_height); }

		inline const float GetSize() const override { return m_width * m_height; }
	};
}