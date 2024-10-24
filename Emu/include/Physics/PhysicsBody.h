#pragma once

#include "../ECS/Component.h"

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

	class PhysicsBody : public Component
	{
	public:
		b2Body* m_body = nullptr;
		// const size_t m_id;
	private:
		BodyType m_bodyType;
		
		// Starting fields for box2d. NOT runtime fields.
		bool m_isSensor;
		float m_restitution;
		float m_restitutionThreshold;
		float m_density;
		float m_friction;

		bool m_fixedRotation;

		// State members
		float m_halfWidth;
		float m_halfHeight;
		float m_width;
		float m_height;

		Vector2D<float> m_startingPosition;
		Vector2D<float> m_prevPosition;
		
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
		EMU_API PhysicsBody(const size_t id);
		EMU_API PhysicsBody(const size_t id, const BodyType bodyType, const bool fixed, const Vector2D<float> position, const Vector2D<float> size);

		EMU_API ~PhysicsBody();

		EMU_API void ApplyForceToBody(Vector2D<float> force);
		EMU_API void ApplyImpulseToBody(Vector2D<float> impulse);

		// PhysicsBody2d getter and setter wrappers
		EMU_API void SetGravity(bool enabled);
		EMU_API void SetXDeceleration(const float xDecel);
		EMU_API void SetXVelocity(const float xVel);
		EMU_API void SetYVelocity(const float yVel);

		EMU_API void SetStartingRestitution(const float restitution);
		EMU_API void SetRestitution(const float restitution);

		EMU_API void SetStartingDensity(const float density);
		EMU_API void SetDensity(const float density);

		EMU_API void SetStartingFriction(const float friction);
		EMU_API void SetFriction(const float friction);

		EMU_API inline Vector2D<float> GetStartingPosition() const { return m_startingPosition; }

		EMU_API inline const float GetHalfWidth() const { return m_halfWidth; }
		EMU_API inline const float GetHalfHeight() const { return m_halfHeight; }

		EMU_API inline const bool GetIsSensor() const { return m_isSensor; }
		EMU_API inline const bool GetIsRotationFixed() const { return m_fixedRotation; }

		EMU_API inline const float GetStartingRestitution() const { return m_restitution; }
		EMU_API inline const float GetStartingRestitutionThreshold() const { return m_restitutionThreshold; }
		EMU_API inline const float GetStartingDensity() const { return m_density; }
		EMU_API inline const float GetStartingFriction() const { return m_friction; }

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
		// PhysicsBody2D specific functions
		void RemoveBodyFromWorld();
		
		void SetFixedRotation(bool fixed);
		void SetIsSensor(const bool sensor);

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
		void Update();

		// Non-PhysicsBody2d getters
		inline const Vector2D<float> GetTopLeftPrevPosition() const { return m_prevPosition; }
		inline const Vector2D<float> GetCenterPrevPosition() const 
		{ return Vector2D<float>(m_prevPosition.X + m_halfWidth, m_prevPosition.Y + m_halfHeight); }
	};
}