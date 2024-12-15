#pragma once

#include "../ECS/Entity.h"
#include "../Core.h"
#include "../MathUtil.h"
#include "ContactSystem.h"

struct b2WorldId;

namespace Engine
{
	// Global utility physics interface
	struct Physics
	{
		// PhysicsBody2d getter and setter wrappers
		EMU_API static void CreateBody(Entity* ptrEntity);
		EMU_API static PhysicsBody* GetBody(Entity* ptrEntity);
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
		EMU_API static void RemoveBodyFromWorld(Entity* ptrEntity);
	};

	// Physics simulation instantiated by scene
	class PhysicsSimulation
	{
	public:
		PhysicsSimulation(const Vector2D<float> gravity);
		// void CreateWorld(const Vector2D<float> gravity);
		void UpdateGravity(const Vector2D<float> gravity);
		void AddPhysicsBodiesToWorld();
		
		void DestroyWorld();
		void Cleanup();

		void Update();
		
	private:
		b2WorldId* m_ptrWorldId;
	};
}