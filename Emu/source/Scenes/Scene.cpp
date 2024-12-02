#pragma once

#include "box2d/box2d.h"

#include "../../include/Scenes/Scene.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Components/PhysicsBody.h"
#include "../../include/CallbackSystem/CallbackSystem.h"
#include "../../include/Tiles/TileMap.h"
#include "../../include/MathUtil.h"
#include "../../include/Components/Transform.h"
#include "../../include/ECS/ECS.h"
#include "../../include/Components/Updatable.h"
#include "../../include/Time.h"
#include "../../include/GameState.h"
#include "../../include/Physics/Physics.h"

namespace Engine
{
	Scene::Scene() : m_levelDimensionsInUnits(32, 32), HasTileMap(false),
		m_tileMap(nullptr),
		refTransformManager(ECS::GetComponentManager<Transform>()),
		refPhysicsBodyManager(ECS::GetComponentManager<PhysicsBody>()),
		refUpdatableManager(ECS::GetComponentManager<Updatable>()) {}

	Scene::~Scene()
	{
		Physics::DestroyWorld();
	}

	void Scene::CheckValid()
	{
		// (m_worldID == nullptr) ? ENGINE_CRITICAL_D("World is nullptr.") : ENGINE_INFO_D("World is valid.");
	}

	void Scene::OnScenePlay()
	{
		Physics::CreateWorld(m_gravity);

		ECS::LoadEntities(m_entities);

		// Physics bodies need to be added to the world after they are activated and pooled.
		Physics::AddPhysicsBodiesToWorld();

		GameState::IN_SCENE = true;
	}

	void Scene::OnSceneEnd()
	{
		GameState::IN_SCENE = false;

		Physics::DestroyWorld();

		ECS::UnloadEntities();
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

	void Scene::Remove(Entity* ptrEntity)
	{
		// Remove entity from the scene. Do not remove the entity from the ECS, just deactivate it.
		m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), ptrEntity), m_entities.end());
		ECS::Deactivate(ptrEntity);
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
		for (Updatable& refUpdatable : refUpdatableManager)
		{
			if (!refUpdatable.IsActive()) continue;
			refUpdatable.Update();
		}

		Physics::Update();

		for (PhysicsBody& refPhysicsBody : refPhysicsBodyManager)
		{
			if (!refPhysicsBody.IsActive()) continue;

			Entity* ptrEntity = refPhysicsBody.GetEntity();

			if (refTransformManager.HasComponent(ptrEntity))
			{
				Transform* ptrTransform = refTransformManager.GetComponent(ptrEntity);
				
				ptrTransform->PrevPosition = ptrTransform->Position;
				ptrTransform->Position = Physics::GetTopLeftPosition(ptrEntity);
				ptrTransform->Dimensions = refPhysicsBody.m_dimensions;
				ptrTransform->Rotation = Physics::GetAngleInDegrees(ptrEntity);
			}
		}
	};
	// Is this function necessary?
	void Scene::CreatePhysicsSimulation(const Vector2D<float> gravity)
	{
		// What happens if this is called multiple times for one scene? Make sure nothing bad.

		ENGINE_INFO_D("Setting gravity: " + std::to_string(gravity.X) + ", " + std::to_string(gravity.Y));

		m_gravity = gravity;

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