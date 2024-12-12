#pragma once

#include "../ECS/Entity.h"
#include "../Core.h"
#include "../MathUtil.h"

#include "ContactSystem.h"

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
		EMU_API static const Vector2D<float> GetVelocity(Entity* ptrEntity);
		EMU_API static void SetRestitution(Entity* ptrEntity, const float restitution);
		EMU_API static void SetDensity(Entity* ptrEntity, const float density);
		EMU_API static void SetFriction(Entity* ptrEntity, const float friction);
		EMU_API static void SetFixedRotation(Entity* ptrEntity, bool fixed);
		EMU_API static const float GetAngleInRadians(Entity* ptrEntity);
		EMU_API static const float GetAngleInDegrees(Entity* ptrEntity);

		static void CreateWorld(const Vector2D<float> gravity);
		static void AddPhysicsBodiesToWorld();

		// PhysicsBody2D specific functions
		// Call sparingly!!!
		static void RemoveBodyFromWorld(Entity* ptrEntity);
		static void DestroyWorld();

		static void Update();
		
	private:
		static b2WorldId* m_ptrWorldId;
	};
}