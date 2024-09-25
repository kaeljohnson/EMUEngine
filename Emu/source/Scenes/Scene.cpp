#pragma once

#include "box2d/box2d.h"

#include "../../include/EngineConstants.h"
#include "../../include/Scenes/Scene.h"
#include "../../include/Scenes/SceneObject.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Physics/PhysicsBody.h"
#include "../../include/CallbackSystem/CallbackSystem.h"
#include "../../include/Tiles/TileMap.h"
#include "../../include/Tiles/Tile.h"
#include "../../include/MathUtil.h"
#include "../../include/Transform.h"
#include "../../include/ComponentManager/ComponentManager.h"

namespace Engine
{
	Scene::Scene() : m_layers(), m_levelDimensionsInUnits(32, 32), HasTileMap(false),
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
			PhysicsBody* ptrBody = reinterpret_cast<PhysicsBody*>(body->GetUserData().pointer);
			ptrBody->RemoveBodyFromWorld();
			body = nextBody;
		}

		delete m_world;
		m_world = nullptr;
	}

	void Scene::CheckValid()
	{
		(m_world == nullptr) ? ENGINE_CRITICAL_D("World is nullptr.") : ENGINE_INFO_D("World is valid.");
		m_layers.size() > 0 ? ENGINE_INFO_D("Scene has at least one layer.") : ENGINE_CRITICAL_D("No layers exist in scene.");
	}

	void Scene::OnScenePlay()
	{
		m_world = new b2World(b2Vec2(m_gravity.X, m_gravity.Y));

		AddPhysicsBodiesToWorld();

		
	}

	void Scene::OnSceneEnd()
	{
		DestroyPhysicsWorld();
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

		for (auto& tile : tileMap)
		{
			Add(tile.m_id, layerIdx);
		}

		for (auto& collisionBody : tileMap.GetCollisionBodies())
		{
			Add(collisionBody.m_id, layerIdx);
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
		// ENGINE_CRITICAL_D("Scene Update!");
		// Faster way to do this? Should only have to update objects
		// prev values if they have changed. In fact, should only update
		// objects that have changed in general

		// Need correct order for updating objects.
		// Dyanmic bodies must be updated after static.

		// Iterate through every layer for now and update.
		// In the future we can filter which layers need to be updated each frame.
		// For instance, if the camera has not moved, we don't need to update the background layer,
		// or the collision bodies layer, and likely not the entire map layer.

		m_world->Step(TIME_STEP, 8, 3);

		for (auto& layer : m_layers)
		{
			for (auto& sceneObjectID : layer)
			{
				// ENGINE_CRITICAL_D(std::to_string(sceneObject->GetUUID()));
				// Update transforms
				Transform* transform = ComponentManagerRegistry::GetManager<Transform>().GetComponent(sceneObjectID);
				PhysicsBody* physicsBody = ComponentManagerRegistry::GetManager<PhysicsBody>().GetComponent(sceneObjectID);

				if (transform == nullptr) ENGINE_CRITICAL_D("Transform is null!");
				if (physicsBody == nullptr) ENGINE_CRITICAL_D("PhysicsBody is null!");

				if (transform != nullptr && physicsBody != nullptr)
				{
					// std::cout << "UHHH: " << transform << ": " << transform->Dimensions.X << "\n";
					/*transform->PrevPosition = sceneObject->GetPhysicsBody()->GetTopLeftPrevPosition();
					transform->Position = sceneObject->GetPhysicsBody()->GetTopLeftPosition();
					transform->Dimensions = sceneObject->GetPhysicsBody()->GetDimensions();
					transform->Rotation = sceneObject->GetPhysicsBody()->GetAngleInDegrees();*/
					physicsBody->Update();

					transform->PrevPosition = physicsBody->GetTopLeftPrevPosition();
					transform->Position = physicsBody->GetTopLeftPosition();
					transform->Dimensions = physicsBody->GetDimensions();
					transform->Rotation = physicsBody->GetAngleInDegrees();
				}
				else
				{
					ENGINE_CRITICAL_D("Transform or PhysicsBody is null!");
				}

				// Update scripts
				// sceneObject->EngineSideUpdate();
				// sceneObject->Update(); // Need scripts to be components

				
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

	void Scene::Add(const size_t id, int layerIdx)
	{
		// Check if layerIdx is valid
		if (layerIdx >= m_layers.size()) 
		{
			ENGINE_CRITICAL_D("Invalid layer index: " + std::to_string(layerIdx) + ". Cannot add SceneObject.");
			return;
		}

		// sceneObject.LayerIdx = layerIdx;
		m_layers[layerIdx].Push(id);
	}

	void Scene::Remove(const size_t id)
	{
		for (auto& layer : m_layers)
		{
			layer.Pop(id);
		}

		/*std::shared_ptr<PhysicsBody> ptrBody = sceneObject.GetPhysicsBody();

		ptrBody->RemoveBodyFromWorld();*/

		// If scene object has a physics body, remove it from the world.
		PhysicsBody* physicsBody = ComponentManagerRegistry::GetManager<PhysicsBody>().GetComponent(id);
		if (physicsBody != nullptr)
		{
			physicsBody->RemoveBodyFromWorld();
		}
	}

	void Scene::AddPhysicsBodiesToWorld()
	{
		// for (auto& layer : m_layers)
		{
			for (auto& physicsBody : ComponentManagerRegistry::GetManager<PhysicsBody>().GetAllComponents())
			{
				if (physicsBody.second != nullptr)
				{
					std::cout << "Adding physics body to world: " << physicsBody.first << "\n";

					if (physicsBody.second == nullptr)
					{
						ENGINE_ERROR_D("PhysicsBody is null!");
						return;
					}

					b2Body* body;
					b2BodyDef bodyDef;
					b2FixtureDef fixtureDef;
					b2PolygonShape shape;
					ENGINE_CRITICAL_D("BRRRRRRRP");
					switch (physicsBody.second->GetBodyType())
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
						physicsBody.second->SetIsSensor(true);
						break;
					default:
						bodyDef.type = b2_staticBody;
						break;
					}

					ENGINE_CRITICAL_D("CHIBCHON");
					bodyDef.fixedRotation = physicsBody.second->GetIsRotationFixed();
					bodyDef.userData.pointer = (uintptr_t)physicsBody.second;
					ENGINE_CRITICAL_D("PLERP");
					bodyDef.position.Set(physicsBody.second->GetStartingPosition().X + physicsBody.second->GetHalfWidth(), physicsBody.second->GetStartingPosition().Y + physicsBody.second->GetHalfHeight());
					ENGINE_CRITICAL_D(std::to_string(physicsBody.second->GetStartingPosition().X));// +physicsBody.second->GetHalfWidth(), physicsBody.second->GetStartingPosition().Y + physicsBody.second->GetHalfHeight()));
					ENGINE_CRITICAL_D("FLIG");
					body = m_world->CreateBody(&bodyDef);

					ENGINE_CRITICAL_D("FUCK");
					shape.SetAsBox(physicsBody.second->GetHalfWidth(), physicsBody.second->GetHalfHeight());
					ENGINE_CRITICAL_D("ME");
					fixtureDef.shape = &shape;
					fixtureDef.restitution = physicsBody.second->GetStartingRestitution();
					ENGINE_CRITICAL_D("5");
					fixtureDef.restitutionThreshold = physicsBody.second->GetStartingRestitutionThreshold();
					ENGINE_CRITICAL_D("6");
					fixtureDef.density = physicsBody.second->GetStartingDensity();
					ENGINE_CRITICAL_D("7");
					fixtureDef.friction = physicsBody.second->GetStartingFriction();
					ENGINE_CRITICAL_D("8");
					fixtureDef.isSensor = physicsBody.second->GetIsSensor();
					ENGINE_CRITICAL_D("9");
					body->CreateFixture(&fixtureDef);

					physicsBody.second->m_body = body;
				}
			}
		}
	}
}