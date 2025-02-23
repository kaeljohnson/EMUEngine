#pragma once

#include "../ECS/ComponentManager.h"
#include "../ECS/ECS.h"
#include "../ECS/Entity.h"
#include "../Tiles/TileMap.h" 
#include "../Physics/Physics.h"
#include "../Camera/CameraInterface.h"
#include "../Includes.h"
#include "../Core.h"
#include "../MathUtil.h"
#include "../Components.h"

struct b2WorldId;

namespace Engine
{
	class Scene
	{
	public:
		EMU_API Scene(ECS& refECS, PhysicsInterface& refPhysicsInterface, CameraInterface& refCameraInterface);
		EMU_API ~Scene();

		using ContactCallback = std::function<void(const Contact&)>;
		EMU_API inline void RegisterContactCallback(ContactType contactType, Entity* ptrEntityA, Entity* ptrEntityB, ContactCallback callback) 
		{ 
			m_physicsSimulation.m_contactSystem.RegisterContactCallback(contactType, ptrEntityA, ptrEntityB, callback); 
		}

		EMU_API inline void RegisterContactCallback(ContactType contactType, Entity* ptrEntity, ContactCallback callback)
		{
			m_physicsSimulation.m_contactSystem.RegisterContactCallback(contactType, ptrEntity, callback);
		}

		EMU_API void SetPhysicsSimulation(const Vector2D<float> gravity);
	
		EMU_API void Add(Entity* ptrEntity);

		// Removes entity from scene by removing entity
		// from entities array and deactivating entity in ECS.
		EMU_API void Activate(Entity* ptrEntity);
		EMU_API void Deactivate(Entity* ptrEntity);

		EMU_API void Remove(Entity* ptrEntity);
		EMU_API void AddTileMap(TileMap& tileMap);

		// IF theres no map in the level, client will decided the dimensions manually.
		EMU_API void SetLevelDimensions(const Vector2D<int> levelWidthInUnits);

	private:
		ECS& m_refECS;

		Vector2D<int> m_levelDimensionsInUnits;
		TileMap* m_tileMap; 

		PhysicsSimulation m_physicsSimulation;
		PhysicsInterface& m_refPhysicsInterface;
		CameraInterface& m_refCameraInterface;

		std::vector<Entity*> m_entities;

		ComponentManager<Transform>& refTransformManager;
		ComponentManager<PhysicsBody>& refPhysicsBodyManager;
		ComponentManager<Updatable>& refUpdatableManager;

	public:
		inline const int GetLevelWidth() const { return m_levelDimensionsInUnits.X; }
		inline const int GetLevelHeight() const { return m_levelDimensionsInUnits.Y; }

		void OnScenePlay();
		void OnSceneEnd();

		void CheckValid();
		void Update();
		void UpdateScripts();
		void UpdatePhysics();
		void UpdateVisuals();

		bool HasTileMap;
	}; 
}