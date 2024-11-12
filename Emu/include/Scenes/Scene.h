#pragma once

#include <memory>

#include "../Core.h"
#include "../MathUtil.h"
#include "../Tiles/TileMap.h" 
#include "../ECS/ComponentManager.h"
#include "../ECS/ECS.h"
#include "../ECS/Entity.h"
#include "../Physics/PhysicsBody.h"
#include "../Updatable/Updatable.h"
#include "../Transform.h"

#include "../GameState.h"

class b2World;

namespace Engine
{
	class Scene
	{
	public:
		EMU_API Scene();
		EMU_API ~Scene();

		EMU_API void CreatePhysicsSimulation(const Vector2D<float> gravity);
		EMU_API void SetGravity(const Vector2D<float> gravity);
	
		EMU_API void Remove(const int entityID);
		EMU_API void AddTileMap(TileMap& tileMap);

		EMU_API void Add(Entity* entity);

		// IF theres no map in the level, client will decided the dimensions manually.
		EMU_API void SetLevelDimensions(const Vector2D<int> levelWidthInUnits);

	private:
		Vector2D<int> m_levelDimensionsInUnits;
		TileMap* m_tileMap;
		b2World* m_world;
		Vector2D<float> m_gravity;

		std::vector<Entity*> m_entities;

		ComponentManager<Transform>& refTransformManager;
		ComponentManager<PhysicsBody>& refPhysicsBodyManager;
		ComponentManager<Updatable>& refUpdatableManager;

	public:
		inline const int GetLevelWidth() const { return m_levelDimensionsInUnits.X; }
		inline const int GetLevelHeight() const { return m_levelDimensionsInUnits.Y; }

		void OnScenePlay();
		void OnSceneEnd();

		void AddPhysicsBodiesToWorld();
		void DestroyPhysicsWorld();

		void CheckValid();
		void Update();

		bool HasTileMap;
	}; 
}