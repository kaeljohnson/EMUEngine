#pragma once

#include "../Core.h"
#include "../MathUtil.h"
#include "../Components.h"
#include "../CharacterTileMap/CharacterTileMap.h" 
#include "ContactSystem.h"

struct b2WorldId;

namespace Engine
{
	// Global utility physics interface
	class PhysicsInterface
	{
	public:
		// PhysicsBody2d getter and setter wrappers
		EMU_API void CreateBody(Entity entity);
		EMU_API const bool HasBody(Entity entity);
		EMU_API void SetBodyType(Entity entity, const BodyType type);
		EMU_API void SetDimensions(Entity entity, const Vector2D<float> dimensions);
		EMU_API const Vector2D<float> GetDimensions(Entity entity);
		EMU_API const Vector2D<float> GetDimensions(PhysicsBody& body);
		EMU_API void SetGravity(Entity entity, bool enabled);
		EMU_API void SetStartingPosition(Entity entity, const Vector2D<float> position);
		EMU_API void SetPosition(Entity entity, const Vector2D<float> position);
		EMU_API const Vector2D<float> GetPosition(Entity entity);
		EMU_API const Vector2D<float> GetTopLeftPosition(Entity entity);
		EMU_API void ApplyForceToBody(Entity entity, Vector2D<float> force);
		EMU_API void ApplyImpulseToBody(Entity entity, Vector2D<float> impulse);
		EMU_API void SetVelocity(Entity entity, const Vector2D<float> velocity);
		EMU_API void SetXVelocity(Entity entity, const float xVelocity);
		EMU_API void SetYVelocity(Entity entity, const float yVelocity);
		EMU_API void SetDeceleration(Entity entity, const float decel);
		EMU_API const Vector2D<float> GetVelocity(Entity entity);
		EMU_API void SetRestitution(Entity entity, const float restitution);
		EMU_API void SetDensity(Entity entity, const float density);
		EMU_API void SetFriction(Entity entity, const float friction);
		EMU_API void SetFixedRotation(Entity entity, bool fixed);
		EMU_API const float GetAngleInRadians(Entity entity);
		EMU_API const float GetAngleInDegrees(Entity entity);
		EMU_API void RemoveBodyFromWorld(Entity entity);

		// Contact System interface
		EMU_API const bool HasContactBelow(Entity entity);
		EMU_API const bool HasContactAbove(Entity entity);
		EMU_API const bool HasContactLeft(Entity entity);
		EMU_API const bool HasContactRight(Entity entity);
	public:
		PhysicsBody* GetBody(Entity entity);
		PhysicsInterface(ECS& refEcs);
	private:
		ECS& m_refECS;
	};

	// Physics simulation instantiated by scene
	class PhysicsSimulation
	{
	public:
		PhysicsSimulation(ECS& refECS, CharacterTileMap& tileMap);
		void CreateWorld(const Vector2D<float> gravity);
		void UpdateGravity(const Vector2D<float> gravity);
		void AddPhysicsBodiesToWorld(std::vector<Entity>& entities);
		void AddPhysicsBodyToWorld(PhysicsBody& refPhysicsBody);
		void AddLineCollidersToWorld(std::vector<Entity>& entities);
		void ActivateContactCallbacks();

		void ActivateBody(Entity entity);
		void ActivateChains(Entity entity);

		void DeactivateBody(Entity entity);
		void DeactivateChains(Entity entity);

		void Update();
		void ProcessSimpleContacts(PhysicsBody& refPhysicsBody);

		void DestroyWorld();
		void Cleanup();

		ContactSystem m_contactSystem;

	private:
		b2WorldId* m_ptrWorldId;
		
		ECS& m_refECS;
	};
}