#pragma once

#include <Engine.h>
#include "../include/EventHandlers/AppManagementEventHandlers.h"
#include "../include/Player/Player.h"
#include "../include/Camera/PlayerCamera.h"

int main(int argc, char* args[])
{
	Engine::Init();
	Engine::EMU* engine = Engine::EMU::GetInstance();

	CLIENT_INFO_D("Client Running!");

	Engine::ScenePtr scene = engine->CreateScene("Level1");
	Engine::ScenePtr scene2 = engine->CreateScene("Level2");

	//// Need physcis to scale with pixels per unit.
	scene->SetPhysicsSimulation(Engine::Vector2D(0.0f, 100.0f));
	scene2->SetPhysicsSimulation(Engine::Vector2D(0.0f, 100.0f));

	// Engine::TileMap testMap(refECS, "testMap1.txt", 1);
	scene->AddTileMap("testMap1.txt", 1);

	Engine::Entity* ptrPlayerEntity = Engine::EMU::GetInstance()->CreateEntity();
	ptrPlayerEntity->SetPriority(0);
	scene->Add(ptrPlayerEntity);

	Player player(ptrPlayerEntity, 6.0f, 1.0f, 0.75f, 0.75f);

	Engine::Entity* ptrTestEntity = Engine::EMU::GetInstance()->CreateEntity();
	ptrTestEntity->SetPriority(1);
	scene->Add(ptrTestEntity);
	engine->AddComponent<Engine::Transform>(ptrTestEntity,
		Engine::Vector2D(12.0f, 12.0f), Engine::Vector2D(1.0f, 1.0f), 1.0f, 1.0f, 1.0f);

	engine->AddComponent<Engine::PhysicsBody>(ptrTestEntity);
	Engine::EMU::GetInstance()->IPHYSICS().SetBodyType(ptrTestEntity, Engine::BodyType::SENSOR);
	Engine::EMU::GetInstance()->IPHYSICS().SetStartingPosition(ptrTestEntity, Engine::Vector2D<float>(12.0f, 64.0f));
	Engine::EMU::GetInstance()->IPHYSICS().SetDimensions(ptrTestEntity, Engine::Vector2D<float>(1.0f, 1.0f));


	scene->RegisterContactCallback(Engine::BEGIN_CONTACT, ptrPlayerEntity, ptrTestEntity, [](const Engine::Contact event)
		{
			CLIENT_INFO_D("Multi Begin Contact");
		});

	scene->RegisterContactCallback(Engine::END_CONTACT, ptrPlayerEntity, ptrTestEntity, [](const Engine::Contact event)
		{
			CLIENT_INFO_D("Multi End Contact");
		});

	scene->RegisterContactCallback(Engine::BEGIN_SENSOR, ptrTestEntity, [](const Engine::Contact event)
		{
			CLIENT_INFO_D("Single Begin Sensing");
		});

	scene->RegisterContactCallback(Engine::END_SENSOR, ptrTestEntity, [](const Engine::Contact event)
		{
			CLIENT_INFO_D("Single End Sensing");
		});

	// Engine::Entity* ptrCameraEntity = Engine::EMU::GetInstance()->IECS().CreateEntity();
	// ptrCameraEntity->SetPriority(0);

	PlayerCamera playerCamera(ptrPlayerEntity);
	// engine->SetCurrentCamera(ptrCameraEntity);
	// scene->Add(ptrCameraEntity);

	scene2->Add(ptrPlayerEntity);

	scene2->AddTileMap("TestMap2.txt", 1);

	engine->LoadScene("Level1");

	AppManagementEventHandlers appManagementEventHandlers(ptrPlayerEntity);

	engine->RunApp();
	// Need to figure out how to change scenes, stop scenes, etc.
	 

	return 0;
}