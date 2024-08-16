#pragma once

#include <memory>

#include "../Core.h"
#include "../MathUtil.h"
#include "../Scenes/SceneObject.h"
#include "../Scenes/SceneObjectStack.h"
#include "../Physics/World.h"
#include "../Tiles/TileMap.h"

namespace Engine
{
	class Scene
	{
	public:
		EMU_API Scene();
		EMU_API ~Scene() = default;

		EMU_API void CreatePhysicsSimulation(const Math::Vector2D<float>, const int pixelsPerUnit);
		EMU_API void SetGravity(const Math::Vector2D<float> gravity);

		// Layer number is the index for now.
		EMU_API void AddLayer(size_t layerIdx);

		EMU_API void Add(SceneObject& sceneObject, int layerNum);
		EMU_API void Remove(SceneObject& sceneObject);

		EMU_API void AddTileMap(TileMap& tileMap, int layerIdx);

		// IF theres no map in the level, client will decide the dimensions manually.
		EMU_API void SetLevelDimensions(const Math::Vector2D<int> levelWidthInUnits);

	private:
		int m_pixelsPerUnit;
		bool m_hasTileMap;

		Math::Vector2D<int> m_levelDimensionsInUnits;
		std::vector<SceneObjectStack> m_layers;
		std::unique_ptr<World> m_world;

	public:
		inline const int GetPixelsPerUnit() const { return m_pixelsPerUnit; }
		inline std::vector<SceneObjectStack>& GetLayers() { return m_layers; }
		inline const int GetLevelWidth() const { return m_levelDimensionsInUnits.X; }
		inline const int GetLevelHeight() const { return m_levelDimensionsInUnits.Y; }

		void CheckValid();
		void OnScenePlay();
		void Update();
	}; 
}