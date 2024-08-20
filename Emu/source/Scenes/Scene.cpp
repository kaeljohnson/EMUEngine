#pragma once

#include "../../include/EngineConstants.h"
#include "../../include/Scenes/Scene.h"
#include "../../include/Scenes/SceneObject.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Physics/PhysicsBody.h"
#include "../../include/CallbackSystem/CallbackSystem.h"
#include "../../include/Tiles/TileMap.h"
#include "../../include/Tiles/Tile.h"
#include "../../include/MathUtil.h"

namespace Engine
{
	Scene::Scene() : m_layers(), m_levelDimensionsInUnits(32, 32), HasTileMap(false),
		m_world(nullptr) {}

	void Scene::CheckValid()
	{
		(m_world == nullptr) ? ENGINE_CRITICAL_D("World is nullptr.") : ENGINE_INFO_D("World is valid.");
		m_layers.size() > 0 ? ENGINE_INFO_D("Scene has at least one layer.") : ENGINE_CRITICAL_D("No layers exist in scene.");
	}

	void Scene::OnScenePlay()
	{
		CheckValid();

	}

	void Scene::AddLayer(size_t layerIdx)
	{
		// Ensure the layer is appended to the end of the vector if the index is out of bounds.
		if (layerIdx > m_layers.size()) 
		{
			ENGINE_CRITICAL_D("Layer index is out of bounds. Add layers sequentially.");
			return;
		}

		m_layers.emplace_back(SceneObjectStack());
	}

	void Scene::AddTileMap(TileMap& tileMap, int layerIdx)
	{
		tileMap.LoadMap();
		tileMap.CreateCollisionBodies();

		m_levelDimensionsInUnits = Vector2D<int>(tileMap.GetWidth(), tileMap.GetHeight());

		ENGINE_CRITICAL_D("Map width: " + std::to_string(m_levelDimensionsInUnits.X) + ", Map height: " + std::to_string(m_levelDimensionsInUnits.Y));

		bool layerExists = false;
		for (auto& tile : tileMap)
		{
			Add(tile, layerIdx);
			layerExists = true;
		}

		if (!layerExists)
		{
			ENGINE_WARN_D("Invalid layer. Cannot add map to scene!");
			return;
		}

		// Tile Maps underlying collision bodies exist in the physics world as continuous bodies,
		// separate from the tile map itself.
		for (auto& tile : tileMap.GetCollisionBodies())
		{
			std::shared_ptr<PhysicsBody> ptrBox = tile.GetPhysicsBody();

			m_world->AddBody(ptrBox);
		}

		HasTileMap = true;
	}

	void Scene::SetLevelDimensions(const Vector2D<int> levelDimensions)
	{
		if (HasTileMap)
		{
			ENGINE_INFO_D("Scene already has a map. Overriding map width!");
		}

		m_levelDimensionsInUnits = levelDimensions;
	}

	void Scene::Update()
	{
		// Faster way to do this? Should only have to update objects
		// prev values if they have changed. In fact, should only update
		// objects that have changed in general

		// Need correct order for updating objects.
		// Dyanmic bodies must be updated after static.

		// Iterate through every layer for now and update.
		// In the future we can filter which layers need to be updated each frame.
		// For instance, if the camera has not moved, we don't need to update the background layer,
		// or the collision bodies layer, and likely not the entire map layer.
		for (auto& layer : m_layers)
		{
			for (auto& sceneObject : layer)
			{
				sceneObject->EngineSideUpdate();
				sceneObject->Update();
			}
		}

		m_world->Update();
	};

	void Scene::CreatePhysicsSimulation(const Vector2D<float> gravity)
	{
		// What happens if this is called multiple times for one scene? Make sure nothing bad.

		ENGINE_INFO_D("Setting gravity: " + std::to_string(gravity.X) + ", " + std::to_string(gravity.Y));

		if (m_world)
		{
			ENGINE_INFO_D("World already set!");
			return;
		}
		
		// Need a reset function for the world which resets all objects in the world.

		m_world = std::make_unique<World>(gravity.X, gravity.Y, 8, 3);

		if (!HasTileMap)
		{
			ENGINE_INFO_D("No map in the level. Add map or set level dimensions manually.");
		}
		else
		{
			ENGINE_INFO_D("Map exists in the level. Setting level width and height to map width and height.");
		}
	}

	void Scene::SetGravity(const Vector2D<float> gravity)
	{

		m_world->SetGravity(gravity.X, gravity.Y);
	
	}

	void Scene::Add(SceneObject& sceneObject, int layerIdx)
	{
		// Check if layerIdx is valid
		if (layerIdx >= m_layers.size()) 
		{
			ENGINE_CRITICAL_D("Invalid layer index: " + std::to_string(layerIdx) + ". Cannot add SceneObject.");
			return;
		}

		sceneObject.LayerIdx = layerIdx;
		m_layers[layerIdx].Push(&sceneObject);

		std::shared_ptr<PhysicsBody> ptrPhysicsBody = sceneObject.GetPhysicsBody();

		m_world->AddBody(ptrPhysicsBody);
	}

	void Scene::Remove(SceneObject& sceneObject)
	{
		if (sceneObject.LayerIdx >= m_layers.size() || sceneObject.LayerIdx == -1)
		{
			ENGINE_CRITICAL_D("Invalid layer index: " + std::to_string(sceneObject.LayerIdx) + ". Cannot remove SceneObject because it does not exist in a valid layer.");
			return;
		}

		m_layers[sceneObject.LayerIdx].Pop(&sceneObject);

		std::shared_ptr<PhysicsBody> ptrBody = sceneObject.GetPhysicsBody();

		ptrBody->RemoveBodyFromWorld();
	}
}