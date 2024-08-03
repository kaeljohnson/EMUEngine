#pragma once

#include <memory>

#include "../Core.h"
#include "../MathUtil.h"

#include "../Scenes/SceneObject.h"
#include "../Scenes/SceneObjectStack.h"
#include "../Physics/IWorld.h"
#include "../Tiles/TileMap.h"

namespace Engine
{
	class Scene
	{
	public:
		EMU_API Scene();
		EMU_API ~Scene() = default;

		EMU_API void SetSimulation(const float gravityX, const float gravityY, const int pixelsPerMeter);

		// Adds scene object to the scene. Need to add a "order" parameter to determine the order of rendering.
		EMU_API void Add(SceneObject& sceneObject, int layerNum);
	
		// Layer number is the index for now.
		EMU_API void AddLayer(size_t layerIdx);
		EMU_API void Remove(SceneObject& sceneObject);
		EMU_API void AddTileMap(TileMap& tileMap, int layerIdx);

		// IF theres no map in the level, client will decided the dimensions manually.
		EMU_API void SetLevelWidthInMeters(const int levelWidthInMeters);
		EMU_API void SetLevelHeightInMeters(const int levelHeightInMeters);

	private:
		int m_pixelsPerMeter;

		// int m_mapWidthInMeters;
		// int m_mapHeightInMeters;
		Vector2D<int> m_mapDimensions;

		// float m_gravityX;
		// float m_gravityY;

		Vector2D<float> m_gravity;

		std::vector<SceneObjectStack> m_layers;

		std::unique_ptr<IWorld> m_world;

	public:
		inline const int GetPixelsPerMeter() const { return m_pixelsPerMeter; }
		inline std::vector<SceneObjectStack>& GetLayers() { return m_layers; }
		inline const int GetLevelWidthInMeters() const { return m_mapDimensions.X; }
		inline const int GetLevelHeightInMeters() const { return m_mapDimensions.Y; }

		void CheckValid();
		void Update();

		bool HasTileMap;
	}; 
}