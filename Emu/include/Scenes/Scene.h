#pragma once

#include <memory>

#include "../Core.h"
#include "../MathUtil.h"

#include "../Containers/IntStack.h"
#include "../Tiles/TileMap.h" 

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

		// Adds scene object to the scene. Need to add a "order" parameter to determine the order of rendering.
		// EMU_API void Add(const int sceneObjectID);
	
		EMU_API void Remove(const int sceneObjectID);
		EMU_API void AddTileMap(TileMap& tileMap);

		// IF theres no map in the level, client will decided the dimensions manually.
		EMU_API void SetLevelDimensions(const Vector2D<int> levelWidthInUnits);

	private:
		Vector2D<int> m_levelDimensionsInUnits;
		TileMap* m_tileMap;
		b2World* m_world;
		Vector2D<float> m_gravity;

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