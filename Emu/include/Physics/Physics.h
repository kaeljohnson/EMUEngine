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
		/**
		* @brief Create a physics body for an entity.
		* 
		* @param entity The entity to create the body for.
		*/
		void CreateBody(Entity entity);

		/**
		* @brief Check if an entity has a physics body.
		* 
		* @param entity The entity to check.
		* 
		* @return True if the entity has a physics body, false otherwise.
		*/
		const bool HasBody(Entity entity);

		/**
		* @brief Set the body type for an entity's physics body.
		* 
		* @param entity The entity to set the body type for.
		* @param type The body type to set.
		*/
		void SetBodyType(Entity entity, const BodyType type);

		/**
		* @brief Get the body type of an entity's physics body.
		* 
		* @param entity The entity to get the body type for.
		* @param dimensions The dimensions to set for the body.
		*/
		void SetDimensions(Entity entity, const Math2D::Point2D<float> dimensions);

		/**
		* @brief Get the dimensions of an entity's physics body.
		* 
		* @param entity The entity to get the dimensions for.
		* 
		* @return The dimensions of the physics body.
		*/
		const Math2D::Point2D<float> GetDimensions(Entity entity);

		/**
		* @brief Get the dimensions of a physics body.
		* 
		* @param body The physics body to get the dimensions for.
		* 
		* @return The dimensions of the physics body.
		*/
		const Math2D::Point2D<float> GetDimensions(PhysicsBody& body);

		/**
		* @brief Enable or disable gravity for an entity's physics body.
		* 
		* @param entity The entity to set gravity for.
		* @param enabled True to enable gravity, false to disable.
		*/
		void SetGravity(Entity entity, bool enabled);

		/**
		* @brief Set the starting position for an entity's physics body.
		* 
		* @param entity The entity to set the starting position for.
		* @param position The starting position to set.
		*/
		void SetStartingPosition(Entity entity, const Math2D::Point2D<float> position);

		/**
		* @brief Set the position for an entity's physics body.
		*
		* @param entity The entity to set the position for.
		* @param position The position to set.
		*/
		void SetPosition(Entity entity, const Math2D::Point2D<float> position);

		/**
		* @brief Get the position of an entity's physics body.
		* 
		* @param entity The entity to get the position for.
		* 
		* @return The position of the physics body.
		*/
		const Math2D::Point2D<float> GetPosition(Entity entity);

		/**
		* @brief Get the top-left position of an entity's physics body.
		* 
		* @param entity The entity to get the top-left position for.
		* 
		* @return The top-left position of the physics body.
		*/
		const Math2D::Point2D<float> GetTopLeftPosition(Entity entity);

		/**
		* @brief Apply a force to an entity's physics body.
		* 
		* @param entity The entity to apply the force to.
		* @param force The force to apply.
		*/
		void ApplyForceToBody(Entity entity, Math2D::Point2D<float> force);

		/**
		* @brief Apply an impulse to an entity's physics body.
		* 
		* @param entity The entity to apply the impulse to.
		* @param impulse The impulse to apply.
		*/
		void ApplyImpulseToBody(Entity entity, Math2D::Point2D<float> impulse);

		/**
		* @brief Set the velocity for an entity's physics body.
		* 
		* @param entity The entity to set the velocity for.
		* @param velocity The velocity to set.
		*/
		void SetVelocity(Entity entity, const Math2D::Point2D<float> velocity);

		/**
		* @brief Set the x velocity for an entity's physics body.
		* 
		* @param entity The entity to set the x velocity for.
		* @param xVelocity The x velocity to set.
		*/
		void SetXVelocity(Entity entity, const float xVelocity);

		/**
		* @brief Set the y velocity for an entity's physics body.
		* 
		* @param entity The entity to set the y velocity for.
		* @param yVelocity The y velocity to set.
		*/
		void SetYVelocity(Entity entity, const float yVelocity);

		/**
		* @brief Set the linear damping for an entity's physics body.
		* 
		* @param entity The entity to set the linear damping for.
		* 
		* @param decel The linear damping to set.
		*/
		void SetDeceleration(Entity entity, const float decel);

		/**
		* @brief Get the velocity of an entity's physics body.
		* 
		* @param entity The entity to get the velocity for.
		* 
		* @return The velocity of the physics body.
		*/
		const Math2D::Point2D<float> GetVelocity(Entity entity);

		/**
		* @brief Set the restitution, density, and friction for an entity's physics body.
		* 
		* @param entity The entity to set the properties for.
		* @param restitution The restitution to set.
		*/
		void SetRestitution(Entity entity, const float restitution);

		/**
		* @brief Set the density for an entity's physics body.
		* 
		* @param entity The entity to set the density for.
		* @param density The density to set.
		*/
		void SetDensity(Entity entity, const float density);

		/**
		* @brief Set the friction for an entity's physics body.
		* 
		* @param entity The entity to set the friction for.
		* @param friction The friction to set.
		*/
		void SetFriction(Entity entity, const float friction);

		/**
		* @brief Set whether the rotation of an entity's physics body is fixed.
		* 
		* @param entity The entity to set the fixed rotation for.
		* @param fixed True to fix rotation, false to allow rotation.
		*/
		void SetFixedRotation(Entity entity, bool fixed);

		/**
		* @brief Set the angle for an entity's physics body in degrees.
		* 
		* @param entity The entity to set the angle for.
		* 
		* @return The angle in radians.
		*/
		const float GetAngleInRadians(Entity entity);

		/**
		* @brief Get the angle for an entity's physics body in degrees.
		* 
		* @param entity The entity to get the angle for.
		* 
		* @return The angle in degrees.
		*/
		const float GetAngleInDegrees(Entity entity);

		/**
		* @brief Remove the physics body of an entity from the world.
		* 
		* @param entity The entity to remove the body for.
		*/
		void RemoveBodyFromWorld(Entity entity);

		/**
		* @brief Check for contact below the entity's physics body.
		* 
		* @param entity The entity to check for contact.
		* 
		* @return True if there is contact below, false otherwise.
		*/
		const bool HasContactBelow(Entity entity);

		/**
		* @brief Check for contact above the entity's physics body.
		* 
		* @param entity The entity to check for contact.
		* 
		* @return True if there is contact above, false otherwise.
		*/
		const bool HasContactAbove(Entity entity);\

		/**
		* @brief Check for contact to the left of the entity's physics body.
		* 
		* @param entity The entity to check for contact.
		* 
		* @return True if there is contact to the left, false otherwise.
		*/
		const bool HasContactLeft(Entity entity);

		/**
		* @brief Check for contact to the right of the entity's physics body.
		* 
		* @param entity The entity to check for contact.
		* 
		* @return True if there is contact to the right, false otherwise.
		*/
		const bool HasContactRight(Entity entity);
	
		/**
		* @brief Get the physics body of an entity.
		* 
		* @param entity The entity to get the body for.
		* 
		* @return Pointer to the physics body.
		*/
		PhysicsBody* GetBody(Entity entity);

		/**
		* @brief Constructor
		* 
		* @param refEcs Reference to the ECS instance.
		*/
		PhysicsInterface(ECS& refEcs);
	private:
		ECS& m_refECS;
	};

	/**
	* @class PhysicsSimulation
	* 
	* @brief Manages the physics simulation using Box2D.
	*/
	class PhysicsSimulation
	{
	public:
		/**
		* @brief Constructor
		* 
		* @param refECS Reference to the ECS
		* @param tileMap Reference to the TileMap
		*/
		PhysicsSimulation(ECS& refECS, TileMap& tileMap);

		/**
		* @brief Create box2D world.
		*/
		void CreateWorld();

		/**
		* @brief Update the world's gravity vector.
		* 
		* @param gravity Gravity vector.
		*/
		void UpdateGravity(const Math2D::Point2D<float> gravity);

		/**
		* @brief Add physics bodies from entities to the world.
		* 
		* @param entities Vector of entities.
		*/
		void AddPhysicsBodiesToWorld(std::unordered_set<Entity>& entities);

		/**
		* @brief Add a single physics body to the world.
		* 
		* @param refPhysicsBody Reference to physics body
		*/
		void AddPhysicsBodyToWorld(PhysicsBody& refPhysicsBody);

		/**
		* @brief Add chain colliders to the world.
		*/
		void AddChainCollidersToWorld();

		/**
		* @brief Activate contact callbacks in contact system.
		*/
		void ActivateContactCallbacks();

		/**
		* @brief Activate physics bodies and chains in the world.
		* 
		* @param entity Entity to activate.
		*/
		void ActivateBody(Entity entity);

		/**
		* @brief Activate physics chains in the world.
		* 
		* @param entity Entity to activate.
		*/
		void ActivateChains(Entity entity);

		/**
		* @brief Deactivate physics bodies and chains in the world.
		* @param entity Entity to deactivate.
		*/
		void DeactivateBody(Entity entity);

		/**
		* @brief Deactivate physics chains in the world.
		* @param entity Entity to deactivate.
		*/
		void DeactivateChains(Entity entity);

		/**
		* @brief Update the physics world.
		*/
		void Update();

		/**
		* @brief Process simple contacts for a physics body.
		* 
		* @param refPhysicsBody Reference to physics body.
		*/
		void ProcessSimpleContacts(PhysicsBody& refPhysicsBody);

		/**
		* @brief Destroy the physics world.
		*/
		void DestroyWorld();

		/**
		* @brief Cleanup the physics simulation.
		*/
		void Cleanup();

		ContactSystem m_contactSystem;	/// Contact system for handling collisions

	private:
		b2WorldId* m_ptrWorldId;			/// Pointer to the Box2D world ID
		
		ECS& m_refECS;						/// Reference to the ECS

		Math2D::Point2D<float> m_gravity;	/// Gravity vector
	};
}