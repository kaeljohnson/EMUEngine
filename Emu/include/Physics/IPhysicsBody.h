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
		virtual void SetWidthInMeters(const float width) = 0;
		virtual void SetHeightInMeters(const float height) = 0;

		virtual const float GetXVelocity() const = 0;
		virtual const float GetYVelocity() const = 0;
		virtual const float GetTopLeftPrevX() const = 0;
		virtual const float GetTopLeftPrevY() const = 0;
		virtual const float GetCenterXInMeters() const = 0;
		virtual const float GetCenterYInMeters() const = 0;
		virtual const float GetTopLeftXInMeters() const = 0;
		virtual const float GetTopLeftYInMeters() const = 0;

		virtual const float GetWidthInMeters() const = 0;
		virtual const float GetHeightInMeters() const = 0;
		virtual const float GetSizeInMeters() const = 0;

		virtual const float GetAngleInRadians() const = 0;
		virtual const float GetAngleInDegrees() const = 0;

		virtual const BodyType GetBodyType() const = 0;

		virtual void RemoveBodyFromWorld() = 0;
		
		virtual void ApplyForceToBox(std::pair<float, float> force) = 0;
		virtual void ApplyImpulseToBox(std::pair<float, float> impulse) = 0;

		virtual bool OnGround() const = 0;
	};
}