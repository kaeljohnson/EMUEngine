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
		EMU_API void CreateBody(Entity* ptrEntity);
		EMU_API void SetBodyType(Entity* ptrEntity, const BodyType type);
		EMU_API void SetDimensions(Entity* ptrEntity, const Vector2D<float> dimensions);
		EMU_API void SetGravity(Entity* ptrEntity, bool enabled);
		EMU_API void SetStartingPosition(Entity* ptrEntity, const Vector2D<float> position);
		EMU_API void SetPosition(Entity* ptrEntity, const Vector2D<float> position);
		EMU_API const Vector2D<float> GetPosition(Entity* ptrEntity);
		EMU_API const Vector2D<float> GetTopLeftPosition(Entity* ptrEntity);
		EMU_API void ApplyForceToBody(Entity* ptrEntity, Vector2D<float> force);
		EMU_API void ApplyImpulseToBody(Entity* ptrEntity, Vector2D<float> impulse);
		EMU_API void SetVelocity(Entity* ptrEntity, const Vector2D<float> velocity);
		EMU_API void SetXVelocity(Entity* ptrEntity, const float xVelocity);
		EMU_API void SetYVelocity(Entity* ptrEntity, const float yVelocity);
		EMU_API void SetDeceleration(Entity* ptrEntity, const float decel);
		EMU_API const Vector2D<float> GetVelocity(Entity* ptrEntity);
		EMU_API void SetRestitution(Entity* ptrEntity, const float restitution);
		EMU_API void SetDensity(Entity* ptrEntity, const float density);
		EMU_API void SetFriction(Entity* ptrEntity, const float friction);
		EMU_API void SetFixedRotation(Entity* ptrEntity, bool fixed);
		EMU_API const float GetAngleInRadians(Entity* ptrEntity);
		EMU_API const float GetAngleInDegrees(Entity* ptrEntity);
		EMU_API void RemoveBodyFromWorld(Entity* ptrEntity);

		// Contact System interface
		EMU_API const bool HasContactBelow(Entity* ptrEntity);
		EMU_API const bool HasContactAbove(Entity* ptrEntity);
		EMU_API const bool HasContactLeft(Entity* ptrEntity);
		EMU_API const bool HasContactRight(Entity* ptrEntity);
	public:
		PhysicsBody* GetBody(Entity* ptrEntity);
		SimpleContact* GetSimpleContact(Entity* ptrEntity);
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