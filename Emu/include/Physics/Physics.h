#pragma once

#include "../ECS/Entity.h"

#include "../Core.h"

#include "../MathUtil.h"

struct b2WorldId;

namespace Engine
{
	class Physics
	{
	public: 
		

		// PhysicsBody2d getter and setter wrappers
		EMU_API static void SetGravity(Entity* ptrEntity, bool enabled);
		EMU_API static void SetPosition(Entity* ptrEntity, const Vector2D<float> position);
		EMU_API static const Vector2D<float> GetPosition(Entity* ptrEntity);
		EMU_API static const Vector2D<float> GetTopLeftPosition(Entity* ptrEntity);
		EMU_API static void ApplyForceToBody(Entity* ptrEntity, Vector2D<float> force);
		EMU_API static void ApplyImpulseToBody(Entity* ptrEntity, Vector2D<float> impulse);
		EMU_API static void SetVelocity(Entity* ptrEntity, const Vector2D<float> velocity);
		EMU_API static void SetXVelocity(Entity* ptrEntity, const float xVelocity);
		EMU_API static void SetYVelocity(Entity* ptrEntity, const float yVelocity);
		EMU_API static void SetDeceleration(Entity* ptrEntity, const float decel);
		EMU_API static Vector2D<int> GetVelocity(Entity* ptrEntity);

		// EMU_API void SetStartingRestitution(const float restitution);
		EMU_API static void SetRestitution(Entity* ptrEntity, const float restitution);

		// EMU_API void SetStartingDensity(const float density);
		EMU_API static void SetDensity(Entity* ptrEntity, const float density);

		// EMU_API void SetStartingFriction(const float friction);
		EMU_API static void SetFriction(Entity* ptrEntity, const float friction);

		// EMU_API inline Vector2D<float> GetStartingPosition() const { return m_startingPosition; }

		// EMU_API inline const float GetHalfWidth() const { return m_halfWidth; }
		// EMU_API inline const float GetHalfHeight() const { return m_halfHeight; }

		// EMU_API inline const bool GetIsSensor() const { return m_isSensor; }
		// EMU_API inline const bool GetIsRotationFixed() const { return m_fixedRotation; }

		// EMU_API inline const float GetStartingRestitution() const { return m_restitution; }
		// EMU_API inline const float GetStartingRestitutionThreshold() const { return m_restitutionThreshold; }
		// EMU_API inline const float GetStartingDensity() const { return m_density; }
		// EMU_API inline const float GetStartingFriction() const { return m_friction; }

		// EMU_API inline const bool GetHasCollisionBelow() const { return m_bottomCollision; }
		// EMU_API inline const bool GetHasCollisionAbove() const { return m_topCollision; }
		// EMU_API inline const bool GetHasCollisionLeft() const { return m_leftCollision; }
		// EMU_API inline const bool GetHasCollisionRight() const { return m_rightCollision; }

		// EMU_API inline const bool GetHasSensorBelow() const { return m_bottomSensor; }
		// EMU_API inline const bool GetHasSensorAbove() const { return m_topSensor; }
		// EMU_API inline const bool GetHasSensorLeft() const { return m_leftSensor; }
		// EMU_API inline const bool GetHasSensorRight() const { return m_rightSensor; }

		/*EMU_API const Vector2D<float> GetVelocity(Entity* ptrEntity);
		EMU_API const Vector2D<float> GetCenterPosition(Entity* ptrEntity);
		EMU_API const Vector2D<float> GetTopLeftPosition(Entity* ptrEntity);*/

		// EMU_API inline const Vector2D<float> GetDimensions() const { return Vector2D<float>(m_width, m_height); }

		// EMU_API inline const float GetSize() const { return m_width * m_height; }

		// EMU_API void OnDeactivate() override;
		// EMU_API void OnActivate() override;
		// EMU_API void OnUnload() override;

		// PhysicsBody2D specific functions
		// Call sparingly!!!
		static void RemoveBodyFromWorld(Entity* ptrEntity);
		// void SetPointersToNull();

		// May want non-runtime activation and deactivation functions.

		// void AddCollidingBody(Entity* body, const ContactDirection contactDirection);
		// void RemoveCollidingBody(Entity* body);
		// void AddSensorContact(Entity* entity);
		// void RemoveSensorContact(Entity* entity);

		static void SetFixedRotation(Entity* ptrEntity, bool fixed);

		// void SetContactFlags();
		// void SetContactFlags(const bool leftCollision, const bool topCollision, const bool rightCollision, const bool bottomCollision);
		// void SetSensorFlags(const bool leftSensor, const bool topSensor, const bool rightSensor, const bool bottomSensor);
		// void SetContactFlagsToFalse();

		// void SetBottomCollision(const bool bottomCollision);
		// void SetTopCollision(const bool topCollision);
		// void SetLeftCollision(const bool leftCollision);
		// void SetRightCollision(const bool rightCollision);

		// void SetBottomSensor(const bool bottomSensor);
		// void SetTopSensor(const bool topSensor);
		// void SetLeftSensor(const bool leftSensor);
		// void SetRightSensor(const bool rightSensor);

		// inline const BodyType GetBodyType() const { return m_bodyType; }

		static const float GetAngleInRadians(Entity* ptrEntity);
		static const float GetAngleInDegrees(Entity* ptrEntity);

		static void Update();
		static void ProcessContactEvents();

		static void AddPhysicsBodiesToWorld();

		static void CreateWorld(const Vector2D<float> gravity);
		static void DestroyWorld();

	private:
		static b2WorldId* m_ptrWorldId;
	};
}