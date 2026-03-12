#pragma once

#include "../../Public/MathUtil.h"
#include "../Components.h"
#include "../TileMap/TileMap.h" 
#include "ContactSystem.h"
#include "PhysicsInterface.h"

struct b2WorldId;

namespace Engine
{
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
		PhysicsSimulation(ECS& refECS);

		void AddPhysicsTileMap(TileMap* tileMap);

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
		void AddPhysicsBodiesToWorld(std::unordered_map<Entity, bool>& entities);

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