#pragma once

#include "box2d/box2d.h"
#include "../../include/CharacterTileMap/CharacterTileMap.h"
#include "../../include/Physics/Physics.h"
#include "../../include/Scenes/Scene.h"
#include "../../include/Logging/Logger.h"
#include "../../include/MathUtil.h"
#include "../../include/ECS/ECS.h"
#include "../../include/Time.h"
#include "../../include/AppState.h"

namespace Engine
{
	Scene::Scene(ECS& refECS, AssetManager& refAssetManager)
		: m_refECS(refECS), m_levelDimensionsInUnits(32, 32), HasTileMap(false), m_tileMap(m_refECS, refAssetManager), 
		m_physicsSimulation(refECS, m_tileMap), 
		m_cameraSystem(refECS)
	{
		m_entities.reserve(10000);
	}

	Scene::~Scene()
	{
		m_physicsSimulation.Cleanup();
	}
	
	void Scene::RegisterContactCallback(ContactType contactType, const char entityA, const char entityB, ContactCallback callback)
	{
		m_physicsSimulation.m_contactSystem.RegisterContactCallback(contactType, entityA, entityB, callback);
	}

	void Scene::RegisterContactCallback(ContactType contactType, const char entity, ContactCallback callback)
	{
		m_physicsSimulation.m_contactSystem.RegisterContactCallback(contactType, entity, callback);
	}

	void Scene::RegisterOnScenePlay(std::function<void()> func)
	{
		// Register a function to be called when the scene starts playing.
		// This can be used to initialize things that need to be set up when the scene starts.
		m_clientOnScenePlay = func;
	}

	void Scene::OnScenePlay()
	{
		// Order matters here.

		// 1. Create the world.
		m_physicsSimulation.CreateWorld(Vector2D<float>(0.0f, 100.0f)); //TODO: Client sets gravity.

		// 2. Load audio files for the tile map.
		m_tileMap.LoadAudioFiles();

		// 3. Load the map. Adds components defined in the rules file and adds them to the ECS.
		m_tileMap.LoadMap();

		// 4. Activate the entities in the ECS. This will activate all components in the ECS.
		// m_refECS.ActivateEntities(m_entities);

		// 5. Frame the cameras, deactivate all but first camera.
		std::vector<Camera>& activeCameras = m_refECS.GetHotComponents<Camera>();

		if (activeCameras.size() == 0)
		{
			ENGINE_CRITICAL_D("No active cameras in the scene. Cannot frame camera.");
			std::runtime_error("No active cameras in the scene. Cannot frame camera.");
			return;
		}

		for (size_t idx = 0; idx < activeCameras.size(); idx++)
		{
			m_cameraSystem.Frame(activeCameras[idx], Vector2D<int>(GetLevelWidth(), GetLevelHeight()));
		}

		// 6. Physics bodies need to be added to the world after they are activated and pooled.
		m_physicsSimulation.AddPhysicsBodiesToWorld(m_entities);
		m_physicsSimulation.AddLineCollidersToWorld(m_entities);

		// 7. Contact callbacks need to be activated.
		m_physicsSimulation.ActivateContactCallbacks();

		// 8. Deactivate all components that should not be active at the start of the scene?

		ENGINE_CRITICAL_D("Num transforms: " + std::to_string(m_refECS.GetHotComponents<Transform>().size()) + ", Num bodies: "
			+ std::to_string(m_refECS.GetHotComponents<PhysicsBody>().size()) + ", Num line colliders: "
			+ std::to_string(m_refECS.GetHotComponents<ChainCollider>().size()) + ", Num cameras: "
			+ std::to_string(m_refECS.GetHotComponents<Camera>().size()) + ", Num tile map entities: "
			+ std::to_string(m_tileMap.m_allMapEntities.size()));

		// process items client wants to do.
		m_clientOnScenePlay();

		AppState::IN_SCENE = true;
	}

	void Scene::OnSceneEnd()
	{
		// Could be problematic if this is called mid frame.
		AppState::IN_SCENE = false;

		m_physicsSimulation.Cleanup();

		// Deactivate all entities and destroy all components.
		m_refECS.DeactivateEntities();
		m_refECS.DestroyComponents(m_entities);
	}

	void Scene::AddTileMap(std::string mapFileName, std::string rulesFileName)
	{
		// Get a temp vector or tile IDs from the tile map. Both the transforms and the physics bodies.
		m_tileMap.CreateMap(mapFileName, rulesFileName);

		// ENGINE_INFO_D("Tile map text file size: " + std::to_string(m_tileMap.m_map.size()));

		m_levelDimensionsInUnits = Vector2D<int>(m_tileMap.GetWidth(), m_tileMap.GetHeight());

		ENGINE_CRITICAL_D("Map width: " + std::to_string(m_levelDimensionsInUnits.X) + ", Map height: " 
			+ std::to_string(m_levelDimensionsInUnits.Y));

		HasTileMap = true;

		for (auto& pair : m_tileMap.m_allMapEntities)
		{	
			Add(pair.first);
		}
	}

	void Scene::Add(Entity entity)
	{
		m_entities.push_back(entity);
	}

	void Scene::Activate(Entity entity)
	{
		ActivatePhysics(entity);
	}

	void Scene::ActivatePhysics(Entity entity)
	{
		m_physicsSimulation.ActivateBody(entity);
		m_physicsSimulation.ActivateChains(entity);
	}

	void Scene::Deactivate(Entity entity)
	{
		DeactivatePhysics(entity);
	}

	void Scene::DeactivatePhysics(Entity entity)
	{
		m_physicsSimulation.DeactivateBody(entity);
		m_physicsSimulation.DeactivateChains(entity);
	}

	void Scene::Remove(Entity entity)
	{
		// Remove entity from the scene. Do not remove the entity from the ECS, just deactivate it.
		m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity), m_entities.end());
		m_refECS.Deactivate(entity);
	}

	void Scene::SetLevelDimensions(const Vector2D<int> levelDimensions)
	{
		if (HasTileMap)
		{
			ENGINE_INFO_D("Scene already has a map. Overriding map width!");
		}

		m_levelDimensionsInUnits = levelDimensions;
	}

	void Scene::UpdatePhysics()
	{
		m_physicsSimulation.Update();
	}

	void Scene::UpdateCamera(AssetManager& refAssetManager)
	{
		m_cameraSystem.Update(refAssetManager);
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

	Entity Scene::GetTileMapEntity(char tileChar) const
	{
		return m_tileMap.GetEntity(tileChar);
	}

	std::vector<Entity> Scene::GetTileMapEntities(const char tileChar) const
	{
		return m_tileMap.GetEntities(tileChar);
	}
}