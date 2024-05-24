#pragma once

#include <utility>

#include "../Core.h"

#include "BodyTypes.h"

// 2D Rigid body interface for Box2D Body.

namespace Engine
{
	// Client will not be able to create an instance of this class in the future.
	class EMU_API IPhysicsBody
	{
	public:
		virtual ~IPhysicsBody() = default;

		virtual void createFixture() = 0;
		virtual const float getCenterXInMeters() const = 0;
		virtual const float getCenterYInMeters() const = 0;
		virtual const float getTopLeftXInMeters() const = 0;
		virtual const float getTopLeftYInMeters() const = 0;

		virtual const float getWidthInMeters() const = 0;
		virtual const float getHeightInMeters() const = 0;

		virtual const float getAngleInRadians() const = 0;
		virtual const float getAngleInDegrees() const = 0;

		virtual const BodyType getBodyType() const = 0;

		virtual const float getPrevX() const = 0;
		virtual const float getPrevY() const = 0;

		virtual void updatePrevX() = 0;
		virtual void updatePrevY() = 0;

		virtual void bodyNotInWorldAlert() const = 0;
		virtual void removeBodyFromWorld() = 0;

		virtual void SetXDeceleration(const float xDecel) = 0;

		virtual void SetXVelocity(const float xVel) = 0;
		virtual void SetYVelocity(const float yVel) = 0;
		virtual const float GetXVelocity() const = 0;
		virtual const float GetYVelocity() const = 0;

		virtual void SetGravity(bool enabled) = 0;

		virtual void ApplyForceToBox(std::pair<float, float> force) = 0;
		virtual void ApplyImpulseToBox(std::pair<float, float> impulse) = 0;
	};
}