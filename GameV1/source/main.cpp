#pragma once

#include <Engine.h>
#include "../include/EventHandlers/AppManagementEventHandlers.h"
#include "../include/Player/Player.h"
#include "../include/Camera/PlayerCamera.h"

int main(int argc, char* args[])
{
	Engine::Init(10000);
	Engine::EMU* engine = Engine::EMU::GetInstance();

	CLIENT_INFO_D("Client Running!");

	Engine::ScenePtr scene = engine->CreateScene("Level1");
	Engine::ScenePtr scene2 = engine->CreateScene("Level2");

	//// Need physcis to scale with pixels per unit.
	scene->SetPhysicsSimulation(Engine::Vector2D(0.0f, 100.0f));
	scene2->SetPhysicsSimulation(Engine::Vector2D(0.0f, 100.0f));

	// Engine::TileMap testMap(refECS, "testMap1.txt", 1);
	scene->AddTileMap("testMap1.txt", 1);

	Engine::Entity testEntity = Engine::EMU::GetInstance()->CreateEntity();
	// ptrTestEntity->SetPriority(1);
	scene->Add(testEntity);
	engine->AddComponent<Engine::Transform>(testEntity,
		Engine::Vector2D(12.0f, 12.0f), Engine::Vector2D(1.0f, 1.0f), 1.0f, 1.0f, 1.0f, 2);

	engine->AddComponent<Engine::PhysicsBody>(testEntity);
	Engine::EMU::GetInstance()->IPHYSICS().SetBodyType(testEntity, Engine::BodyType::SENSOR);
	Engine::EMU::GetInstance()->IPHYSICS().SetStartingPosition(testEntity, Engine::Vector2D<float>(12.0f, 64.0f));
	Engine::EMU::GetInstance()->IPHYSICS().SetDimensions(testEntity, Engine::Vector2D<float>(1.0f, 1.0f));

	Engine::Entity playerEntity = Engine::EMU::GetInstance()->CreateEntity();
	// ptrPlayerEntity->SetPriority(0);
	scene->Add(playerEntity);

	Player player(playerEntity, 6.0f, 1.0f, 0.75f, 0.75f);
	

	scene->RegisterContactCallback(Engine::BEGIN_CONTACT, playerEntity, testEntity, [](const Engine::Contact event)
		{
			CLIENT_INFO_D("Multi Begin Contact");
		});

	scene->RegisterContactCallback(Engine::END_CONTACT, playerEntity, testEntity, [](const Engine::Contact event)
		{
			CLIENT_INFO_D("Multi End Contact");
		});

	scene->RegisterContactCallback(Engine::BEGIN_SENSOR, testEntity, [](const Engine::Contact event)
		{
			CLIENT_INFO_D("Single Begin Sensing");
		});

	scene->RegisterContactCallback(Engine::END_SENSOR, testEntity, [](const Engine::Contact event)
		{
			CLIENT_INFO_D("Single End Sensing");
		});

	// Engine::Entity* ptrCameraEntity = Engine::EMU::GetInstance()->IECS().CreateEntity();
	// ptrCameraEntity->SetPriority(0);

	PlayerCamera playerCamera(playerEntity);
	// engine->SetCurrentCamera(ptrCameraEntity);
	// scene->Add(ptrCameraEntity);

	scene2->Add(playerEntity);

	scene2->AddTileMap("TestMap2.txt", 1);


	engine->LoadScene("Level1");

	AppManagementEventHandlers appManagementEventHandlers(playerEntity, testEntity);

	engine->RunApp();
	// Need to figure out how to change scenes, stop scenes, etc.
	 

	return 0;
}