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
		* @brief Constructs the PhysicsSimulation with a reference to the ECS.
		* 
		* @param refECS Reference to the ECS.
		*/
		PhysicsSimulation(ECS& refECS);

		/**
		* @brief Create the Box2D world.
		*/
		void CreateWorld();

		/**
		* @brief Destroy the physics world.
		*/
		void DestroyWorld();

		/**
		* @brief Cleanup the physics simulation.
		*/
		void Cleanup();

		/**
		* @brief Update the world's gravity vector.
		* 
		* @param gravity Gravity vector.
		*/
		void UpdateGravity(const Math2D::Point2D<float> gravity);

		/**
		* @brief Gets the current gravity vector.
		* 
		* @return A const reference to the gravity vector.
		*/
		const Math2D::Point2D<float>& GetGravity() const { return m_gravity; }

		/**
		* @brief Adds a TileMap pointer for a specific layer number to the contact system.
		* 
		* @param layerNum The layer number for which the TileMap is being added.
		* @param ptrTileMap Pointer to the TileMap to be added for the specified layer.
		*/
		void AddPhysicsTileMap(size_t layerNum, TileMap* ptrTileMap) { m_contactSystem.AddTileMap(layerNum, ptrTileMap); }

		/**
		* @brief Add physics bodies from entities to the world.
		* 
		* @param entities Vector of entities.
		*/
		void AddPhysicsBodiesToWorld(std::unordered_map<Entity, bool>& entities);

		/**
		* @brief Add a single physics body to the world.
		* 
		* @param refPhysicsBody Reference to physics body.
		*/
		void AddPhysicsBodyToWorld(PhysicsBody& refPhysicsBody);

		/**
		* @brief Add chain colliders to the world.
		*/
		void AddChainCollidersToWorld();

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
		* 
		* @param entity Entity to deactivate.
		*/
		void DeactivateBody(Entity entity);

		/**
		* @brief Deactivate physics chains in the world.
		* 
		* @param entity Entity to deactivate.
		*/
		void DeactivateChains(Entity entity);

		/**
		* @brief Activate contact callbacks in contact system.
		*/
		void ActivateContactCallbacks();

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

		ContactSystem m_contactSystem;	/// Contact system for handling collisions

	private:
		b2WorldId* m_ptrWorldId;			/// Pointer to the Box2D world ID

		ECS& m_refECS;						/// Reference to the ECS

		Math2D::Point2D<float> m_gravity;	/// Gravity vector
	};
}