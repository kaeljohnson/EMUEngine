#pragma once

#include "../MathUtil.h"
#include "../Components.h"
#include "../TileMap/TileMap.h" 
#include "ContactSystem.h"

struct b2WorldId;

namespace Engine
{
	// Global utility physics interface
	class PhysicsInterface
	{
	public:
		// PhysicsBody2d getter and setter wrappers
		void CreateBody(Entity entity);
		const bool HasBody(Entity entity);
		void SetBodyType(Entity entity, const BodyType type);
		void SetDimensions(Entity entity, const Vector2D<float> dimensions);
		const Vector2D<float> GetDimensions(Entity entity);
		const Vector2D<float> GetDimensions(PhysicsBody& body);
		void SetGravity(Entity entity, bool enabled);
		void SetStartingPosition(Entity entity, const Vector2D<float> position);
		void SetPosition(Entity entity, const Vector2D<float> position);
		const Vector2D<float> GetPosition(Entity entity);
		const Vector2D<float> GetTopLeftPosition(Entity entity);
		void ApplyForceToBody(Entity entity, Vector2D<float> force);
		void ApplyImpulseToBody(Entity entity, Vector2D<float> impulse);
		void SetVelocity(Entity entity, const Vector2D<float> velocity);
		void SetXVelocity(Entity entity, const float xVelocity);
		void SetYVelocity(Entity entity, const float yVelocity);
		void SetDeceleration(Entity entity, const float decel);
		const Vector2D<float> GetVelocity(Entity entity);
		void SetRestitution(Entity entity, const float restitution);
		void SetDensity(Entity entity, const float density);
		void SetFriction(Entity entity, const float friction);
		void SetFixedRotation(Entity entity, bool fixed);
		const float GetAngleInRadians(Entity entity);
		const float GetAngleInDegrees(Entity entity);
		void RemoveBodyFromWorld(Entity entity);

		// Contact System interface
		const bool HasContactBelow(Entity entity);
		const bool HasContactAbove(Entity entity);
		const bool HasContactLeft(Entity entity);
		const bool HasContactRight(Entity entity);
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
		PhysicsSimulation(ECS& refECS, TileMap& tileMap);

		/*
			Create box2D world with specified gravity vector.
			arg1: gravity vector
		*/
		void CreateWorld(const Vector2D<float> gravity);

		/*
			Update the world's gravity vector.
			arg1: gravity vector
		*/
		void UpdateGravity(const Vector2D<float> gravity);

		/*
			Add physics bodies from entities to the world.
			arg1: vector of entities
		*/
		void AddPhysicsBodiesToWorld(std::vector<Entity>& entities);

		/*
			Add a single physics body to the world.
			arg1: reference to physics body
		*/
		void AddPhysicsBodyToWorld(PhysicsBody& refPhysicsBody);

		/*
			Add line colliders from entities to the world.
			arg1: vector of entities
		*/
		void AddLineCollidersToWorld(std::vector<Entity>& entities);

		/*
			Activate contact callbacks in contact system.
		*/
		void ActivateContactCallbacks();

		/*
			Activate physics bodies and chains in the world.
			arg1: entity to activate
		*/
		void ActivateBody(Entity entity);

		/*
			Activate physics chains in the world.
			arg1: entity to activate
		*/
		void ActivateChains(Entity entity);

		/*
			Deactivate physics bodies and chains in the world.
			arg1: entity to deactivate
		*/
		void DeactivateBody(Entity entity);

		/*
			Deactivate physics chains in the world.
			arg1: entity to deactivate
		*/
		void DeactivateChains(Entity entity);

		/*
			Update the physics world.
		*/
		void Update();

		/*
			Process simple contacts for a physics body.
			arg1: reference to physics body
		*/
		void ProcessSimpleContacts(PhysicsBody& refPhysicsBody);

		/*
			Destroy the physics world.
		*/
		void DestroyWorld();

		/*
			Cleanup the physics simulation.
		*/
		void Cleanup();

		ContactSystem m_contactSystem;

	private:
		b2WorldId* m_ptrWorldId;
		
		ECS& m_refECS;
	};
}