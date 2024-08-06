#pragma once

#include "../Core.h"

#include "BodyTypes.h"

// 2D Rigid body interface for Box2D Body.

namespace Engine
{
	// Client will not be able to create an instance of this class in the future.
	class IPhysicsBody
	{
	public:
		virtual ~IPhysicsBody() = default;

		virtual void CreateFixture() = 0;

		virtual void UpdatePrevPosition() = 0;

		virtual void SetDensity(const float density) = 0;
		virtual void SetFriction(const float friction) = 0;
		virtual void SetRestitution(const float restitution) = 0;
		virtual void SetRestitutionThreshold(const float threshold) = 0;
		virtual void SetCollidable(const bool collidable) = 0;
		virtual void SetGravity(bool enabled) = 0;
		virtual void SetFixedRotation(bool fixed) = 0;
		virtual void SetXDeceleration(const float xDecel) = 0;

		virtual void SetXVelocity(const float xVel) = 0;
		virtual void SetYVelocity(const float yVel) = 0;

		virtual void SetWidth(const float width) = 0;
		virtual void SetHeight(const float height) = 0;

		virtual const Vector2D<float> GetVelocity() const = 0;
		virtual const Vector2D<float> GetTopLeftPrevPosition() const = 0;
		virtual const Vector2D<float> GetCenterPrevPosition() const = 0;
		virtual const Vector2D<float> GetCenterPosition() const = 0;
		virtual const Vector2D<float> GetTopLeftPosition() const = 0; 
		virtual const Vector2D<float> GetDimensions() const = 0;

		virtual const float GetSize() const = 0;

		virtual const float GetAngleInRadians() const = 0;
		virtual const float GetAngleInDegrees() const = 0;

		virtual const BodyType GetBodyType() const = 0;

		virtual void RemoveBodyFromWorld() = 0; 
		
		virtual void ApplyForceToBody(Vector2D<float> force) = 0;
		virtual void ApplyImpulseToBody(Vector2D<float> impulse) = 0;

		virtual void SetContactFlags() = 0;
		virtual void SetContactFlagsToFalse() = 0;

		virtual void SetBottomCollision(const bool bottomCollision) = 0;
		virtual void SetTopCollision(const bool topCollision) = 0;
		virtual void SetLeftCollision(const bool leftCollision) = 0;
		virtual void SetRightCollision(const bool rightCollision) = 0;

		virtual void SetBottomSensor(const bool bottomSensor) = 0;
		virtual void SetTopSensor(const bool topSensor) = 0;
		virtual void SetLeftSensor(const bool leftSensor) = 0;
		virtual void SetRightSensor(const bool rightSensor) = 0;

		virtual const bool GetHasSensorBelow() const = 0;
		virtual const bool GetHasSensorAbove() const = 0;
		virtual const bool GetHasSensorLeft() const = 0;
		virtual const bool GetHasSensorRight() const = 0;

		virtual const bool GetHasCollisionBelow() const = 0;
		virtual const bool GetHasCollisionAbove() const = 0;
		virtual const bool GetHasCollisionLeft() const = 0;
		virtual const bool GetHasCollisionRight() const = 0;
	};
}