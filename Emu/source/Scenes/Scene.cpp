#pragma once

#include "box2d/box2d.h"
#include "../../include/Tiles/TileMap.h"
#include "../../include/Physics/Physics.h"
#include "../../include/Scenes/Scene.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Components.h"
#include "../../include/Camera/CameraInterface.h"
#include "../../include/MathUtil.h"
#include "../../include/ECS/ECS.h"
#include "../../include/Time.h"
#include "../../include/GameState.h"

namespace Engine
{
	Scene::Scene(ECS& refECS, PhysicsInterface& refPhysicsInterface, CameraInterface& refCameraInterface)
		: m_refECS(refECS), m_levelDimensionsInUnits(32, 32), HasTileMap(false), m_tileMap(nullptr), 
		m_physicsSimulation(refECS, Vector2D<float>(0.0f, 100.0f)), 
		m_cameraSystem(refECS),
		m_updateSystem(refECS),
		m_refPhysicsInterface(refPhysicsInterface),
		refTransformManager(refECS.GetComponentManager<Transform>()),
		refPhysicsBodyManager(refECS.GetComponentManager<PhysicsBody>()),
		refUpdatableManager(refECS.GetComponentManager<Updatable>()) {}

	Scene::~Scene()
	{
		m_physicsSimulation.Cleanup();
	}

	void Scene::CheckValid()
	{
		// (m_worldID == nullptr) ? ENGINE_CRITICAL_D("World is nullptr.") : ENGINE_INFO_D("World is valid.");
	}

	void Scene::OnScenePlay()
	{
		// m_physicsSimulation.CreateWorld(m_gravity);

		m_refECS.LoadEntities(m_entities);

		// Frame the first active camera
		for (auto& camera : m_refECS.GetComponentManager<Camera>())
		{
			if (camera.IsActive())
			{
				m_cameraSystem.Frame(camera, Vector2D<int>(GetLevelWidth(), GetLevelHeight()));
				break;
			}
		}

		// Physics bodies need to be added to the world after they are activated and pooled.
		m_physicsSimulation.AddPhysicsBodiesToWorld();

		GameState::IN_SCENE = true;
	}

	void Scene::OnSceneEnd()
	{
		GameState::IN_SCENE = false;

		m_physicsSimulation.Cleanup();

		m_refECS.UnloadEntities();
	}

	void Scene::AddTileMap(TileMap& tileMap)
	{
		// Get a temp vector or tile IDs from the tile map. Both the transforms and the physics bodies.
		std::vector<Entity*> tileMapEntities = tileMap.LoadMap();
		std::vector<Entity*> mapCollisionBodies = tileMap.CreateCollisionBodies();

		ENGINE_INFO_D("Tile map text file size: " + std::to_string(tileMap.m_map.size()));
		ENGINE_INFO_D("Tile map collision bodies size: " + std::to_string(tileMap.m_collisionBodies.size()));

		m_levelDimensionsInUnits = Vector2D<int>(tileMap.GetWidth(), tileMap.GetHeight());

		ENGINE_CRITICAL_D("Map width: " + std::to_string(m_levelDimensionsInUnits.X) + ", Map height: " 
			+ std::to_string(m_levelDimensionsInUnits.Y));

		HasTileMap = true;

		for (auto& entity : tileMapEntities)
		{
			Add(entity);
		}

		for (auto& entity : mapCollisionBodies)
		{
			Add(entity);
		}
	}

	void Scene::Add(Entity* ptrEntity)
	{
		m_entities.push_back(ptrEntity);
	}

	void Scene::Activate(Entity* ptrEntity)
	{
		m_refECS.Activate(ptrEntity);
	}

	void Scene::Deactivate(Entity* ptrEntity)
	{
		m_refECS.Deactivate(ptrEntity);
	}

	void Scene::Remove(Entity* ptrEntity)
	{
		// Remove entity from the scene. Do not remove the entity from the ECS, just deactivate it.
		m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), ptrEntity), m_entities.end());
		m_refECS.Deactivate(ptrEntity);
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
		m_updateSystem.Update();

		m_physicsSimulation.Update();

		for (PhysicsBody& refPhysicsBody : refPhysicsBodyManager)
		{
			if (!refPhysicsBody.IsActive()) continue;

			Entity* ptrEntity = refPhysicsBody.GetEntity();

			if (refTransformManager.HasComponent(ptrEntity))
			{
				Transform* ptrTransform = refTransformManager.GetComponent(ptrEntity);
				
				ptrTransform->PrevPosition = ptrTransform->Position;
				ptrTransform->Position = m_refPhysicsInterface.GetTopLeftPosition(ptrEntity);
				ptrTransform->Dimensions = refPhysicsBody.m_dimensions;
				ptrTransform->Rotation = m_refPhysicsInterface.GetAngleInDegrees(ptrEntity);
			}
		}
	};

	void Scene::UpdateScripts()
	{
		for (Updatable& refUpdatable : refUpdatableManager)
		{
			if (!refUpdatable.IsActive()) continue;
			refUpdatable.Update();
		}
	}

	void Scene::UpdatePhysics()
	{
		m_physicsSimulation.Update();

		for (PhysicsBody& refPhysicsBody : refPhysicsBodyManager)
		{
			if (!refPhysicsBody.IsActive()) continue;

			Entity* ptrEntity = refPhysicsBody.GetEntity();

			if (refTransformManager.HasComponent(ptrEntity))
			{
				Transform* ptrTransform = refTransformManager.GetComponent(ptrEntity);

				ptrTransform->PrevPosition = ptrTransform->Position;
				ptrTransform->Position = m_refPhysicsInterface.GetTopLeftPosition(ptrEntity);
				ptrTransform->Dimensions = refPhysicsBody.m_dimensions;
				ptrTransform->Rotation = m_refPhysicsInterface.GetAngleInDegrees(ptrEntity);
			}
		}
	}

	void Scene::UpdateCamera()
	{
		m_cameraSystem.Update();
	}

	void Scene::UpdateVisuals()
	{
		
	}

	// Is this function necessary?
	void Scene::SetPhysicsSimulation(const Vector2D<float> gravity)
	{
		// What happens if this is called multiple times for one scene? Make sure nothing bad.

		ENGINE_INFO_D("Setting gravity: " + std::to_string(gravity.X) + ", " + std::to_string(gravity.Y));

		// m_gravity = gravity;

		m_physicsSimulation.UpdateGravity(gravity);

		// Need a reset function for the world which resets all objects in the world.

		if (!HasTileMap)
		{
			ENGINE_INFO_D("No map in the level. Add map or set level dimensions manually.");
		}
		else
		{
			ENGINE_INFO_D("Map exists in the level. Setting level width and height to map width and height.");
		}
	}
}