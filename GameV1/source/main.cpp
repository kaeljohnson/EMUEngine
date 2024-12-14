#pragma once

#include <Engine.h>
#include "../include/EventHandlers/AppManagementEventHandlers.h"
#include "../include/Player/Player.h"
#include "../include/Camera/PlayerCamera.h"

struct TestContactListener : public Engine::MultiEntityContactListener
{
	TestContactListener(Engine::Entity* ptrEntity1, Engine::Entity* ptrEntity2) 
		: Engine::MultiEntityContactListener(ptrEntity1, ptrEntity2) {}

	void OnContactBegin(const Engine::Contact event) override
	{
		CLIENT_INFO_D("Contact Begin");
	}

	void OnContactEnd(const Engine::Contact event) override
	{
		CLIENT_INFO_D("Contact End");
	}
};

struct TestSensorListener : public Engine::MultiEntitySensorListener
{
	TestSensorListener(Engine::Entity* ptrEntity1, Engine::Entity* ptrEntity2) 
		: Engine::MultiEntitySensorListener(ptrEntity1, ptrEntity2) {}

	void OnContactBegin(const Engine::Contact event) override
	{
		CLIENT_INFO_D("Sensor Begin");
	}

	void OnContactEnd(const Engine::Contact event)
	{
		CLIENT_INFO_D("Sensor End");
	}
};

int main(int argc, char* args[])
{
	Engine::Init();

	CLIENT_INFO_D("Client Running!");

	Engine::Application app;
	Engine::CameraManager& refCameraManager = app.GetCameraManager();
	Engine::SceneManager& refSceneManager = app.GetSceneManager();

	Engine::ScenePtr scene = Engine::CreateScene();
	Engine::ScenePtr scene2 = Engine::CreateScene();

	refSceneManager.AddScene("Level1", scene);
	refSceneManager.AddScene("Level2", scene2);

	//// Need physcis to scale with pixels per unit.
	scene->CreatePhysicsSimulation(Engine::Vector2D(0.0f, 100.0f));
	scene2->CreatePhysicsSimulation(Engine::Vector2D(0.0f, 100.0f));

	Engine::TileMap testMap("testMap1.txt", 1);
	scene->AddTileMap(testMap);

	Engine::Entity* ptrPlayerEntity = Engine::ECS::CreateEntity();
	ptrPlayerEntity->SetPriority(0);
	scene->Add(ptrPlayerEntity);

	Player player(ptrPlayerEntity, 6.0f, 1.0f, 0.75f, 0.75f);

	Engine::Entity* ptrTestEntity = Engine::ECS::CreateEntity();
	ptrTestEntity->SetPriority(1);
	scene->Add(ptrTestEntity);
	Engine::ECS::GetComponentManager<Engine::Transform>().AddComponent(ptrTestEntity,
		Engine::Vector2D(12.0f, 12.0f), Engine::Vector2D(1.0f, 1.0f), 1.0f, 1.0f, 1.0f);

	Engine::ECS::GetComponentManager<Engine::PhysicsBody>().AddComponent(ptrTestEntity);
	Engine::PhysicsBody* ptrPhysicsBody =
		Engine::ECS::GetComponentManager<Engine::PhysicsBody>().GetComponent(ptrTestEntity);
	ptrPhysicsBody->m_bodyType = Engine::BodyType::DYNAMIC;
	ptrPhysicsBody->m_startingPosition = Engine::Vector2D<float>(12.0f, 12.0f);
	ptrPhysicsBody->m_dimensions = Engine::Vector2D<float>(1.0f, 1.0f);
	ptrPhysicsBody->m_halfDimensions = ptrPhysicsBody->m_dimensions * 0.5f;
	ptrPhysicsBody->m_bodyType = Engine::BodyType::DYNAMIC;

	TestContactListener testContactListener(ptrPlayerEntity, ptrTestEntity);
	TestSensorListener testSensorListener(ptrPlayerEntity, ptrTestEntity);

	Engine::ContactSystem::RegisterContactListener(&testContactListener);
	Engine::ContactSystem::RegisterContactListener(&testSensorListener);

	Engine::ContactSystem::RegisterContactHandler(Engine::SingleEntityBeginContactKey(ptrTestEntity), [](Engine::Contact event)
		{
			CLIENT_INFO_D("TEST");
		});

	Engine::Entity* ptrCameraEntity = Engine::ECS::CreateEntity();
	ptrCameraEntity->SetPriority(0);

	PlayerCamera playerCamera(ptrCameraEntity, ptrPlayerEntity);
	refCameraManager.SetCurrentCamera(ptrCameraEntity);

	scene2->Add(ptrPlayerEntity);

	Engine::TileMap testMap2("TestMap2.txt", 1);
	scene2->AddTileMap(testMap2);

	refSceneManager.LoadScene("Level1");

	AppManagementEventHandlers appManagementEventHandlers(ptrCameraEntity, ptrPlayerEntity);

	app.Start();
	// Need to figure out how to change scenes, stop scenes, etc.

	return 0;
}