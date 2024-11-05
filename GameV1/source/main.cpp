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
	
	const size_t tileID = Engine::ECS::CreateEntity();
	const size_t tileID2 = Engine::ECS::CreateEntity();

	scene->Add(tileID);
	scene->Add(tileID2);

	const Engine::EntityID playerEntityID = Engine::ECS::CreateEntity();
	scene->Add(playerEntityID);

	Player player(playerEntityID, 6.0f, 1.0f, 0.75f, 0.75f, refEventManager.GetKeyStates());

	const Engine::EntityID cameraEntityID = Engine::ECS::CreateEntity();
	PlayerCamera playerCamera(cameraEntityID, playerEntityID);
	refCameraManager.SetCurrentCamera(cameraEntityID);

	scene2->Add(playerEntityID);

	Engine::TileMap testMap2("TestMap2.txt", 1);
	scene2->AddTileMap(testMap2);

	refSceneManager.LoadScene("Level1");

	AppManagementEventHandlers appManagementEventHandlers(refEventManager, cameraEntityID);

	ptrAppInstance->Start();
	// Need to figure out how to change scenes, stop scenes, etc.

	return 0;
}