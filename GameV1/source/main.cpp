#pragma once

#include <Engine.h>
#include "../include/EventHandlers/AppManagementEventHandlers.h"
#include "../include/Player/Player.h"
#include "../include/Camera/PlayerCamera.h"

int main(int argc, char* args[])
{
	Engine::Init();
	Engine::EMU* engine = Engine::EMU::GetInstance();
	Engine::ECS& refECS = engine->IECS();

	CLIENT_INFO_D("Client Running!");

	Engine::ScenePtr scene = engine->CreateScene("Level1");
	Engine::ScenePtr scene2 = engine->CreateScene("Level2");

	//// Need physcis to scale with pixels per unit.
	scene->SetPhysicsSimulation(Engine::Vector2D(0.0f, 100.0f));
	scene2->SetPhysicsSimulation(Engine::Vector2D(0.0f, 100.0f));

	Engine::TileMap testMap(refECS, "testMap1.txt", 1);
	scene->AddTileMap(testMap);

	Engine::Entity* ptrPlayerEntity = Engine::EMU::GetInstance()->IECS().CreateEntity();
	ptrPlayerEntity->SetPriority(0);
	scene->Add(ptrPlayerEntity);

	Player player(ptrPlayerEntity, 6.0f, 1.0f, 0.75f, 0.75f);

	Engine::Entity* ptrTestEntity = Engine::EMU::GetInstance()->IECS().CreateEntity();
	ptrTestEntity->SetPriority(1);
	scene->Add(ptrTestEntity);
	Engine::EMU::GetInstance()->IECS().GetComponentManager<Engine::Transform>().AddComponent(ptrTestEntity,
		Engine::Vector2D(12.0f, 12.0f), Engine::Vector2D(1.0f, 1.0f), 1.0f, 1.0f, 1.0f);

	Engine::EMU::GetInstance()->IECS().GetComponentManager<Engine::PhysicsBody>().AddComponent(ptrTestEntity);
	Engine::PhysicsBody* ptrPhysicsBody =
		Engine::EMU::GetInstance()->IECS().GetComponentManager<Engine::PhysicsBody>().GetComponent(ptrTestEntity);
	ptrPhysicsBody->m_bodyType = Engine::BodyType::SENSOR;
	ptrPhysicsBody->m_startingPosition = Engine::Vector2D<float>(12.0f, 64.0f);
	ptrPhysicsBody->m_dimensions = Engine::Vector2D<float>(1.0f, 1.0f);
	ptrPhysicsBody->m_halfDimensions = ptrPhysicsBody->m_dimensions * 0.5f;


	Engine::EMU::GetInstance()->CONTACT_SYSTEM().RegisterContactCallback(Engine::BEGIN_CONTACT, ptrPlayerEntity, ptrTestEntity, [](const Engine::Contact event)
		{
			CLIENT_INFO_D("Multi Begin Contact");
		});

	Engine::EMU::GetInstance()->CONTACT_SYSTEM().RegisterContactCallback(Engine::END_CONTACT, ptrPlayerEntity, ptrTestEntity, [](const Engine::Contact event)
		{
			CLIENT_INFO_D("Multi End Contact");
		});

	Engine::EMU::GetInstance()->CONTACT_SYSTEM().RegisterContactCallback(Engine::BEGIN_SENSOR, ptrTestEntity, [](const Engine::Contact event)
		{
			CLIENT_INFO_D("Single Begin Sensing");
		});

	Engine::EMU::GetInstance()->CONTACT_SYSTEM().RegisterContactCallback(Engine::END_SENSOR, ptrTestEntity, [](const Engine::Contact event)
		{
			CLIENT_INFO_D("Single End Sensing");
		});

	Engine::Entity* ptrCameraEntity = Engine::EMU::GetInstance()->IECS().CreateEntity();
	ptrCameraEntity->SetPriority(0);

	PlayerCamera playerCamera(ptrCameraEntity, ptrPlayerEntity);
	engine->SetCurrentCamera(ptrCameraEntity);

	scene2->Add(ptrPlayerEntity);

	Engine::TileMap testMap2(refECS, "TestMap2.txt", 1);
	scene2->AddTileMap(testMap2);

	engine->LoadScene("Level1");

	AppManagementEventHandlers appManagementEventHandlers(ptrCameraEntity, ptrPlayerEntity);

	engine->RunApp();
	// Need to figure out how to change scenes, stop scenes, etc.
	 

	return 0;
}