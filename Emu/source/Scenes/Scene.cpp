#pragma once

#include "box2d/box2d.h"

#include "../../include/EngineConstants.h"
#include "../../include/Scenes/Scene.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Physics/PhysicsBody.h"
#include "../../include/CallbackSystem/CallbackSystem.h"
#include "../../include/Tiles/TileMap.h"
#include "../../include/Tiles/Tile.h"
#include "../../include/MathUtil.h"
#include "../../include/Transform.h"
#include "../../include/ComponentManager/ComponentManager.h"
#include "../../include/ComponentManager/Updatable.h"

namespace Engine
{
	Scene::Scene() : m_levelDimensionsInUnits(32, 32), HasTileMap(false),
		m_world(nullptr), m_tileMap(nullptr) {}

	Scene::~Scene()
	{
		DestroyPhysicsWorld();
	}

	void Scene::DestroyPhysicsWorld()
	{
		if (m_world == nullptr)
		{
			ENGINE_INFO_D("World is already null. No need to destroy.");
			return;
		}

		ENGINE_INFO_D("Freeing World!");
		// Destroy all bodies in the world
		b2Body* body = m_world->GetBodyList();
		while (body != nullptr)
		{
			b2Body* nextBody = body->GetNext();
			PhysicsBody* ptrBody = ComponentManagerRegistry::GetManager<PhysicsBody>().GetComponent(body->GetUserData().pointer);
			ptrBody->RemoveBodyFromWorld();
			body = nextBody;
		}

		delete m_world;
		m_world = nullptr;
	}

	void Scene::CheckValid()
	{
		(m_world == nullptr) ? ENGINE_CRITICAL_D("World is nullptr.") : ENGINE_INFO_D("World is valid.");
	}

	void Scene::OnScenePlay()
	{
		m_world = new b2World(b2Vec2(m_gravity.X, m_gravity.Y));

		auto& updatableManager = ComponentManagerRegistry::GetManager<Updatable>();
		auto& physicsBodyManager = ComponentManagerRegistry::GetManager<PhysicsBody>();
		auto& transformManager = ComponentManagerRegistry::GetManager<Transform>();

		ENGINE_CRITICAL_D("Updatable component vector size: " + std::to_string(updatableManager.GetComponents().size()));
		ENGINE_CRITICAL_D("PhysicsBody component vector size: " + std::to_string(physicsBodyManager.GetComponents().size()));
		ENGINE_CRITICAL_D("Transform component vector size: " + std::to_string(transformManager.GetComponents().size()));

		for (auto& sceneObject : m_sceneObjects)
		{
			updatableManager.ActivateComponent(sceneObject);
			physicsBodyManager.ActivateComponent(sceneObject);
			transformManager.ActivateComponent(sceneObject);
		}

		updatableManager.PoolActiveComponents();
		physicsBodyManager.PoolActiveComponents();
		transformManager.PoolActiveComponents();

		// Physics bodies need to be added to the world after they are activated and pooled.
		AddPhysicsBodiesToWorld();
	}

	void Scene::OnSceneEnd()
	{
		auto& updatableManager = ComponentManagerRegistry::GetManager<Updatable>();
		auto& physicsBodyManager = ComponentManagerRegistry::GetManager<PhysicsBody>();
		auto& transformManager = ComponentManagerRegistry::GetManager<Transform>();

		for (auto& sceneObject : m_sceneObjects)
		{
			updatableManager.DeactivateComponent(sceneObject);
			physicsBodyManager.DeactivateComponent(sceneObject);
			transformManager.DeactivateComponent(sceneObject);
		}

		DestroyPhysicsWorld();
	}

	void Scene::AddTileMap(TileMap& tileMap)
	{
		// Get a temp vector or tile IDs from the tile map. Both the transforms and the physics bodies.
		std::vector<size_t> tileMapEntities = tileMap.LoadMap();
		std::vector<size_t> mapCollisionBodies = tileMap.CreateCollisionBodies();

		ENGINE_INFO_D("Tile map text file size: " + std::to_string(tileMap.m_map.size()));
		ENGINE_INFO_D("Tile map tile map size: " + std::to_string(tileMap.m_tiles.size()));
		ENGINE_INFO_D("Tile map collision bodies size: " + std::to_string(tileMap.m_collisionBodies.size()));

		m_levelDimensionsInUnits = Vector2D<int>(tileMap.GetWidth(), tileMap.GetHeight());

		ENGINE_CRITICAL_D("Map width: " + std::to_string(m_levelDimensionsInUnits.X) + ", Map height: " 
			+ std::to_string(m_levelDimensionsInUnits.Y));

		HasTileMap = true;

		for (auto& id : tileMapEntities)
		{
			Add(id);
		}

		for (auto& id : mapCollisionBodies)
		{
			Add(id);
		}
	}

	void Scene::Add(const size_t sceneObjectID)
	{
		m_sceneObjects.push_back(sceneObjectID);
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
		// Should move some of this logic to ECS itself?
		auto& transforms = ComponentManagerRegistry::GetManager<Transform>().GetComponents();
		auto& physicsBodies = ComponentManagerRegistry::GetManager<PhysicsBody>().GetComponents();
		auto& updatables = ComponentManagerRegistry::GetManager<Updatable>().GetComponents();

		for (auto& updatable : updatables)
		{
			updatable.Update();
			if (updatable.IsLastActive()) break;
		}

		for (auto& physicsBody : physicsBodies)
		{
			physicsBody.Update();
			if (physicsBody.IsLastActive()) break;
		}

		m_world->Step(TIME_STEP, 8, 3);

		for (auto& transform : transforms)
		{	
			if (!transform.IsActive())
			{
				continue;

			}

			PhysicsBody* physicsBody = ComponentManagerRegistry::GetManager<PhysicsBody>().GetComponent(transform.GetID());
			if (physicsBody != nullptr)
			{
				transform.PrevPosition = physicsBody->GetTopLeftPrevPosition();
				transform.Position = physicsBody->GetTopLeftPosition();
				transform.Dimensions = physicsBody->GetDimensions();
				transform.Rotation = physicsBody->GetAngleInDegrees();
			}

			if (transform.IsLastActive()) break;
		}
	};

	void Scene::CreatePhysicsSimulation(const Vector2D<float> gravity)
	{
		// What happens if this is called multiple times for one scene? Make sure nothing bad.

		ENGINE_INFO_D("Setting gravity: " + std::to_string(gravity.X) + ", " + std::to_string(gravity.Y));

		m_gravity = gravity;

		if (m_world)
		{
			ENGINE_INFO_D("World already set!");
			return;
		}
		
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

	void Scene::SetGravity(const Vector2D<float> gravity)
	{

		m_world->SetGravity(b2Vec2(m_gravity.X, m_gravity.Y));
	
	}

	void Scene::Remove(const int id)
	{
		// If scene object has a physics body, remove it from the world.
		PhysicsBody* physicsBody = ComponentManagerRegistry::GetManager<PhysicsBody>().GetComponent(id);
		if (physicsBody != nullptr)
		{
			physicsBody->RemoveBodyFromWorld();
		}
	}

	void Scene::AddPhysicsBodiesToWorld()
	{
		for (auto& physicsBody : ComponentManagerRegistry::GetManager<PhysicsBody>().GetComponents())
		{
			if (!physicsBody.IsActive())
			{
				continue;
			}

			b2Body* body;
			b2BodyDef bodyDef;
			b2FixtureDef fixtureDef;
			b2PolygonShape shape;
			switch (physicsBody.GetBodyType())
			{
				case STATIC:
					bodyDef.type = b2_staticBody;
					break;
				case DYNAMIC:
					bodyDef.type = b2_dynamicBody;
					break;
				case KINEMATIC:
					bodyDef.type = b2_kinematicBody;
					break;
				case SENSOR:
					bodyDef.type = b2_kinematicBody;
					physicsBody.SetIsSensor(true);
					break;
				default:
					bodyDef.type = b2_staticBody;
					break;
			}

			bodyDef.fixedRotation = physicsBody.GetIsRotationFixed();

			bodyDef.userData.pointer = physicsBody.GetID(); // NOTE: This is the entity ID of the physics body. Not a pointer to the physics body.

			bodyDef.position.Set(physicsBody.GetStartingPosition().X + physicsBody.GetHalfWidth(), 
				physicsBody.GetStartingPosition().Y + physicsBody.GetHalfHeight());
					
			body = m_world->CreateBody(&bodyDef);

			shape.SetAsBox(physicsBody.GetHalfWidth(), physicsBody.GetHalfHeight());
			fixtureDef.shape = &shape;
			fixtureDef.restitution = physicsBody.GetStartingRestitution();
			fixtureDef.restitutionThreshold = physicsBody.GetStartingRestitutionThreshold();
			fixtureDef.density = physicsBody.GetStartingDensity();
			fixtureDef.friction = physicsBody.GetStartingFriction();
			fixtureDef.isSensor = physicsBody.GetIsSensor();
			body->CreateFixture(&fixtureDef);

			physicsBody.m_body = body;
		}
	}
}