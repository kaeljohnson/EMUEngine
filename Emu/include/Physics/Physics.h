#pragma once

#include "../ECS/Entity.h"
#include "../Core.h"
#include "../MathUtil.h"
#include "ContactSystem.h"

struct b2WorldId;

namespace Engine
{
	// Global utility physics interface
	class PhysicsInterface
	{
	public:
		// PhysicsBody2d getter and setter wrappers
		EMU_API void CreateBody(Entity* ptrBody);
		EMU_API PhysicsBody* GetBody(Entity* ptrBody);
		EMU_API void SetGravity(PhysicsBody* ptrBody, bool enabled);
		EMU_API void SetPosition(PhysicsBody* ptrBody, const Vector2D<float> position);
		EMU_API const Vector2D<float> GetPosition(PhysicsBody* ptrBody);
		EMU_API const Vector2D<float> GetTopLeftPosition(PhysicsBody* ptrBody);
		EMU_API void ApplyForceToBody(PhysicsBody* ptrBody, Vector2D<float> force);
		EMU_API void ApplyImpulseToBody(PhysicsBody* ptrBody, Vector2D<float> impulse);
		EMU_API void SetVelocity(PhysicsBody* ptrBody, const Vector2D<float> velocity);
		EMU_API void SetXVelocity(PhysicsBody* ptrBody, const float xVelocity);
		EMU_API void SetYVelocity(PhysicsBody* ptrBody, const float yVelocity);
		EMU_API void SetDeceleration(PhysicsBody* ptrBody, const float decel);
		EMU_API const Vector2D<float> GetVelocity(PhysicsBody* ptrBody);
		EMU_API void SetRestitution(PhysicsBody* ptrBody, const float restitution);
		EMU_API void SetDensity(PhysicsBody* ptrBody, const float density);
		EMU_API void SetFriction(PhysicsBody* ptrBody, const float friction);
		EMU_API void SetFixedRotation(PhysicsBody* ptrBody, bool fixed);
		EMU_API const float GetAngleInRadians(PhysicsBody* ptrBody);
		EMU_API const float GetAngleInDegrees(PhysicsBody* ptrBody);
		EMU_API void RemoveBodyFromWorld(PhysicsBody* ptrBody);
	public:
		PhysicsInterface(ECS& refEcs);
	private:
		ECS& m_refECS;
	};

	// Physics simulation instantiated by scene
	class PhysicsSimulation
	{
	public:
		PhysicsSimulation(ECS& refECS, const Vector2D<float> gravity);
		// void CreateWorld(const Vector2D<float> gravity);
		void UpdateGravity(const Vector2D<float> gravity);
		void AddPhysicsBodiesToWorld();

		void Update();

		void DestroyWorld();
		void Cleanup();

		ContactSystem m_contactSystem;

	private:
		b2WorldId* m_ptrWorldId;
		
		ECS& m_refECS;
	};
}