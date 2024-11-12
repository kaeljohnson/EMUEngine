#pragma once

#include "box2d/box2d.h"

#include "../../include/Scenes/Scene.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Physics/PhysicsBody.h"
#include "../../include/CallbackSystem/CallbackSystem.h"
#include "../../include/Tiles/TileMap.h"
#include "../../include/MathUtil.h"
#include "../../include/Transform.h"
#include "../../include/ECS/ECS.h"
#include "../../include/Updatable/Updatable.h"
#include "../../include/Time.h"
#include "../../include/GameState.h"
namespace Engine
{
	Scene::Scene() : m_levelDimensionsInUnits(32, 32), HasTileMap(false),
		m_world(nullptr), m_tileMap(nullptr),
		refTransformManager(ECS::GetComponentManager<Transform>()),
		refPhysicsBodyManager(ECS::GetComponentManager<PhysicsBody>()),
		refUpdatableManager(ECS::GetComponentManager<Updatable>()) {}

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
			PhysicsBody* ptrBody = ECS::GetComponentManager<PhysicsBody>().GetComponent((Entity*)body->GetUserData().pointer);
			ptrBody->RemoveBodyFromWorld();
			body = nextBody;
		}

		delete m_world;
		m_world = nullptr;
		ENGINE_INFO_D("World freed!");
	}

	void Scene::CheckValid()
	{
		(m_world == nullptr) ? ENGINE_CRITICAL_D("World is nullptr.") : ENGINE_INFO_D("World is valid.");
	}

	void Scene::OnScenePlay()
	{
		m_world = new b2World(b2Vec2(m_gravity.X, m_gravity.Y));

		ENGINE_CRITICAL_D("Updatable component vector size: " + std::to_string(refUpdatableManager.GetComponents().size()));
		ENGINE_CRITICAL_D("PhysicsBody component vector size: " + std::to_string(refPhysicsBodyManager.GetComponents().size()));
		ENGINE_CRITICAL_D("Transform component vector size: " + std::to_string(refTransformManager.GetComponents().size()));

		ECS::LoadEntities(m_entities);

		// Physics bodies need to be added to the world after they are activated and pooled.
		AddPhysicsBodiesToWorld();

		GameState::IN_SCENE = true;
	}

	void Scene::OnSceneEnd()
	{
		GameState::IN_SCENE = false;

		DestroyPhysicsWorld();

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

		m_world->Step(Time::GetTimeStep(), 8, 3);

		for (PhysicsBody& refPhysicsBody : refPhysicsBodyManager)
		{
			if (!refPhysicsBody.IsActive()) continue;

			refPhysicsBody.Update();

			Entity* ptrEntity = refPhysicsBody.GetEntity();

			if (refTransformManager.HasComponent(ptrEntity))
			{
				Transform* ptrTransform = refTransformManager.GetComponent(ptrEntity);
				
				ptrTransform->PrevPosition = ptrTransform->Position;
				ptrTransform->Position = refPhysicsBody.GetTopLeftPosition();
				ptrTransform->Dimensions = refPhysicsBody.GetDimensions();
				ptrTransform->Rotation = refPhysicsBody.GetAngleInDegrees();
			}
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

	void Scene::AddPhysicsBodiesToWorld()
	{
		for (PhysicsBody& refPhysicsBody : refPhysicsBodyManager)
		{
			if (!refPhysicsBody.IsActive()) continue;

			b2Body* body;
			b2BodyDef bodyDef;
			b2FixtureDef fixtureDef;
			b2PolygonShape shape;
			switch (refPhysicsBody.GetBodyType())
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
					refPhysicsBody.SetIsSensor(true);
					break;
				default:
					bodyDef.type = b2_staticBody;
					break;
			}

			bodyDef.fixedRotation = refPhysicsBody.GetIsRotationFixed();

			bodyDef.userData.pointer = (uintptr_t)refPhysicsBody.GetEntity(); // NOTE: This is the entity ID of the physics body. Not a pointer to the physics body.

			bodyDef.position.Set(refPhysicsBody.GetStartingPosition().X + refPhysicsBody.GetHalfWidth(),
				refPhysicsBody.GetStartingPosition().Y + refPhysicsBody.GetHalfHeight());
					
			body = m_world->CreateBody(&bodyDef);

			shape.SetAsBox(refPhysicsBody.GetHalfWidth(), refPhysicsBody.GetHalfHeight());
			fixtureDef.shape = &shape;
			fixtureDef.restitution = refPhysicsBody.GetStartingRestitution();
			fixtureDef.restitutionThreshold = refPhysicsBody.GetStartingRestitutionThreshold();
			fixtureDef.density = refPhysicsBody.GetStartingDensity();
			fixtureDef.friction = refPhysicsBody.GetStartingFriction();
			fixtureDef.isSensor = refPhysicsBody.GetIsSensor();
			body->CreateFixture(&fixtureDef);

			refPhysicsBody.m_body = body;
		}
	}
}