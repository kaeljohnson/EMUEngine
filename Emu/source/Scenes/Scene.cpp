#pragma once

#include "../../include/EngineConstants.h"
#include "../../include/Scenes/Scene.h"
#include "../../include/Scenes/SceneObject.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Physics/Box.h"
#include "../../include/Physics/PhysicsFactory.h"
#include "../../include/CallbackSystem/CallbackSystem.h"
#include "../../include/Tiles/TileMap.h"
#include "../../include/Tiles/Tile.h"

// TEMP
#include "../../include/RendererManager.h"

namespace Engine
{
	Scene::Scene() : m_pixelsPerMeter(0), m_gravityX(0), m_gravityY(0), m_layers(), m_mapWidthInMeters(0), m_mapHeightInMeters(0), HasTileMap(false),
		 m_world(nullptr), m_eventListeners(), m_camera(0.0f, 0.0f, RendererManager::GetInstance()->GetFullscreenWidth(), RendererManager::GetInstance()->GetFullscreenHeight(), 0, 0, RendererManager::GetInstance()->GetScaleX(), RendererManager::GetInstance()->GetScaleY()) {}

	void Scene::CheckValid()
	{
		(m_world == nullptr) ? ENGINE_CRITICAL_D("World is nullptr.") : ENGINE_INFO_D("World is valid.");
		m_layers.size() > 0 ? ENGINE_INFO_D("Scene has at least one layer.") : ENGINE_CRITICAL_D("No layers exist in scene.");
		(m_pixelsPerMeter <= 0) ? ENGINE_CRITICAL_D("Pixels per meter is invalid.") : ENGINE_INFO_D("Pixels per meter is valid.");
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

		m_mapWidthInMeters = tileMap.GetWidth();
		m_mapHeightInMeters = tileMap.GetHeight();

		m_camera.SetLevelWidthInMeters(m_mapWidthInMeters);
		m_camera.SetLevelHeightInMeters(m_mapHeightInMeters);

		ENGINE_CRITICAL_D("Map width: " + std::to_string(m_mapWidthInMeters) + ", Map height: " + std::to_string(m_mapHeightInMeters));

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
			std::shared_ptr<Box> ptrBox = std::static_pointer_cast<Box>(tile.GetPhysicsBody());

			m_world->AddBox(ptrBox);
		}

		HasTileMap = true;
	}

	void Scene::SetLevelWidthInMeters(const int levelWidthInMeters)
	{
		if (HasTileMap)
		{
			ENGINE_INFO_D("Scene already has a map. Overriding map width!");
		}

		m_mapWidthInMeters = levelWidthInMeters;
		m_camera.SetLevelWidthInMeters(m_mapWidthInMeters);
	}

	void Scene::SetLevelHeightInMeters(const int levelHeightInMeters)
	{
		ENGINE_INFO_D("Scene already has a map. Overriding map height!");

		m_mapHeightInMeters = levelHeightInMeters;
		m_camera.SetLevelHeightInMeters(m_mapHeightInMeters);
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

	void Scene::SetSimulation(const float gravityX, const float gravityY, const int pixelsPerMeter)
	{
		// What happens if this is called multiple times for one scene? Make sure nothing bad.

		m_pixelsPerMeter = pixelsPerMeter;

		m_gravityX = gravityX;
		m_gravityY = gravityY;

		ENGINE_INFO_D("Setting pixels per meter, time step, gravityX and gravityY at positions: " 
			+ std::to_string(m_pixelsPerMeter) + ", " + std::to_string(m_gravityX) + ", " + std::to_string(m_gravityY));

		if (m_world)
		{
			ENGINE_INFO_D("World already set!");
			return;
		}
		
		// Need a reset function for the world which resets all objects in the world.

		m_world = CreateWorld(m_gravityX * m_pixelsPerMeter, m_gravityY * m_pixelsPerMeter, 8, 3);

		m_camera.SetPixelsPerMeter(m_pixelsPerMeter);

		ENGINE_INFO_D("Client creating simulation with gravity: " + std::to_string(gravityX) + ", " + std::to_string(gravityY));

		if (!HasTileMap)
		{
			ENGINE_INFO_D("No map in the level. Add map or set level dimensions manually.");
		}
		else
		{
			ENGINE_INFO_D("Map exists in the level. Setting level width and height to map width and height.");
		}
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

		std::shared_ptr<Box> ptrBox = std::static_pointer_cast<Box>(sceneObject.GetPhysicsBody());

		m_world->AddBox(ptrBox);
	}

	void Scene::Remove(SceneObject& sceneObject)
	{
		if (sceneObject.LayerIdx >= m_layers.size() || sceneObject.LayerIdx == -1)
		{
			ENGINE_CRITICAL_D("Invalid layer index: " + std::to_string(sceneObject.LayerIdx) + ". Cannot remove SceneObject because it does not exist in a valid layer.");
			return;
		}

		m_layers[sceneObject.LayerIdx].Pop(&sceneObject);

		std::shared_ptr<Box> ptrBox = std::static_pointer_cast<Box>(sceneObject.GetPhysicsBody());

		ptrBox->RemoveBodyFromWorld();
	}

	void Scene::AddEventListener(EventListener& eventListener)
	{
		m_eventListeners.Push(&eventListener);
	}

	void Scene::RemoveEventListener(EventListener& eventListener)
	{
		m_eventListeners.Pop(&eventListener);
	}
}