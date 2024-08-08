#pragma once

#include "../Core.h"
#include "../MathUtil.h"

#include "ConversionFunctions.h"

#include "BodyTypes.h"

class b2Body;

namespace Engine
{
	// This class primarily serves as a wrapper for the Box2D b2Body class. It will be used by the 
	// engine as the object that adheres to physics and collission detection. This class
	// is not meant to be used by the client. It is not the "entity" or "game object" class. It will not handle
	// textures or animation.

	class PhysicsBody
	{
	public:
		const BodyType m_bodyType;

		// Box2D specific members.
		b2Body* m_body;

		// Needed fields from box2d
		bool m_isSensor;
		float m_restitution;
		float m_restitutionThreshold;
		float m_density;
		float m_friction;

		bool m_fixedRotation;

		// State members
		const float m_halfWidth;
		const float m_halfHeight;
		const float m_width;
		const float m_height;

		const Vector2D<float> m_startingPosition;
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
		EMU_API void ApplyForceToBody(Vector2D<float> force);
		EMU_API void ApplyImpulseToBody(Vector2D<float> impulse);

		// PhysicsBody2d getter and setter wrappers
		void CreateFixture();
		EMU_API void SetGravity(bool enabled);
		EMU_API void SetXDeceleration(const float xDecel);
		EMU_API void SetXVelocity(const float xVel);
		EMU_API void SetYVelocity(const float yVel);

		EMU_API inline const bool GetHasCollisionBelow() const { return m_bottomCollision; }
		EMU_API inline const bool GetHasCollisionAbove() const { return m_topCollision; }
		EMU_API inline const bool GetHasCollisionLeft() const { return m_leftCollision; }
		EMU_API inline const bool GetHasCollisionRight() const { return m_rightCollision; }

		EMU_API inline const bool GetHasSensorBelow() const { return m_bottomSensor; }
		EMU_API inline const bool GetHasSensorAbove() const { return m_topSensor; }
		EMU_API inline const bool GetHasSensorLeft() const { return m_leftSensor; }
		EMU_API inline const bool GetHasSensorRight() const { return m_rightSensor; }

		EMU_API const Vector2D<float> GetVelocity() const;
		EMU_API const Vector2D<float> GetCenterPosition() const;
		EMU_API const Vector2D<float> GetTopLeftPosition() const;

		EMU_API inline const Vector2D<float> GetDimensions() const { return Vector2D<float>(m_width, m_height); }

		EMU_API inline const float GetSize() const { return m_width * m_height; }

	public:
		PhysicsBody() = default;
		PhysicsBody(const BodyType bodyType, const bool fixed, const Vector2D<float> position, const Vector2D<float> size);

		~PhysicsBody();

		// PhysicsBody2D specific functions
		void RemoveBodyFromWorld();
		
		void SetFixedRotation(bool fixed);

		void SetContactFlags();
		void SetContactFlagsToFalse();

		void SetBottomCollision(const bool bottomCollision);
		void SetTopCollision(const bool topCollision);
		void SetLeftCollision(const bool leftCollision);
		void SetRightCollision(const bool rightCollision);

		void SetBottomSensor(const bool bottomSensor);
		void SetTopSensor(const bool topSensor);
		void SetLeftSensor(const bool leftSensor);
		void SetRightSensor(const bool rightSensor);

		inline const BodyType GetBodyType() const { return m_bodyType; }

		const float GetAngleInRadians() const;
		const float GetAngleInDegrees() const;

		// PhysicsBody2D does not track previous values.
		// Need to update them here.
		void UpdatePrevPosition();

		// Non-PhysicsBody2d getters
		inline const Vector2D<float> GetTopLeftPrevPosition() const { return m_prevPosition; }
		inline const Vector2D<float> GetCenterPrevPosition() const { return Vector2D<float>(m_prevPosition.X + m_halfWidth, m_prevPosition.Y + m_halfHeight); }
	};
}