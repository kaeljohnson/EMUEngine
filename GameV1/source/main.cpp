#pragma once

#include <Engine.h>
#include "../include/EventHandlers/AppManagementEventHandlers.h"
#include "../include/Player/Player.h"
#include "../include/Camera/PlayerCamera.h"

int main(int argc, char* args[])
{
	Engine::Init();

	CLIENT_INFO_D("Client Running!");

	Engine::ApplicationPtr ptrAppInstance = Engine::Application::GetInstance();
	Engine::EventManager& refEventManager = ptrAppInstance->GetEventManager();
	Engine::CameraManager& refCameraManager = ptrAppInstance->GetCameraManager();
	Engine::SceneManager& refSceneManager = ptrAppInstance->GetSceneManager();

	Engine::ScenePtr scene = Engine::CreateScene();
	Engine::ScenePtr scene2 = Engine::CreateScene();

	refSceneManager.AddScene("Level1", scene);
	refSceneManager.AddScene("Level2", scene2);


	//// Need physcis to scale with pixels per unit.
	scene->CreatePhysicsSimulation(Engine::Vector2D(0.0f, 100.0f));
	scene2->CreatePhysicsSimulation(Engine::Vector2D(0.0f, 100.0f));

	Engine::TileMap testMap("testMap1.txt", 1);
	scene->AddTileMap(testMap);
	
	const Engine::Entity playerEntity = Engine::EntityManager::CreateEntity();
	scene->Add(playerEntity);
	CLIENT_CRITICAL_D("Player ID: " + std::to_string(playerEntity));
	Player player(playerEntity, 6.0f, 1.0f, 0.75f, 0.75f, refEventManager.GetKeyStates());

	const Engine::Entity player2ID = Engine::EntityManager::CreateEntity();
	Player player2(player2ID, 2.0f, 2.0f, 0.75f, 0.75f, refEventManager.GetKeyStates());
	// scene2->Add(player2ID);

	//Engine::ScrollingCamera scrollCamera;
	//scrollCamera.SetScrollingSpeeds(Engine::Vector2D<float>(0.0005f, 0.0f));
	//scrollCamera.SetCameraPosition(Engine::Vector2D<float>(0.0f, 38.5f));

	PlayerCamera playerCamera;
	playerCamera.SetPixelsPerUnit(32);
	playerCamera.SetCameraTarget(Engine::ComponentManagerRegistry::GetManager<Engine::Transform>().GetComponent(playerEntity));

	refCameraManager.SetCurrentCamera(&playerCamera);

	//// scene2->AddLayer(MAP_LAYER);
	//// scene2->AddLayer(PLAYER_LAYER);

	scene2->Add(playerEntity);

	Engine::TileMap testMap2("TestMap2.txt", 1);
	scene2->AddTileMap(testMap2);
	

	refSceneManager.LoadScene("Level1");

	AppManagementEventHandlers appManagementEventHandlers(refEventManager, playerCamera);

	ptrAppInstance->Start();
	// Need to figure out how to change scenes, stop scenes, etc.

	return 0;
}