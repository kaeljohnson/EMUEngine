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
	

	// Need physcis to scale with pixels per unit.
	scene->CreatePhysicsSimulation(Engine::Vector2D(0.0f, 100.0f));
	scene2->CreatePhysicsSimulation(Engine::Vector2D(0.0f, 100.0f));
	
	const size_t playerID = Engine::IDGenerator::CreateUUID();
	Player player(playerID, 6.0f, 1.0f, 0.75f, 0.75f, refEventManager.GetKeyStates());
	// const size_t player2ID = Engine::IDGenerator::CreateUUID();
	// Player player2(player2ID, 2.0f, 2.0f, 0.75f, 0.75f, refEventManager.GetKeyStates());

	Engine::ScrollingCamera scrollCamera;
	scrollCamera.SetScrollingSpeeds(Engine::Vector2D<float>(0.0005f, 0.0f));
	scrollCamera.SetCameraPosition(Engine::Vector2D<float>(0.0f, 38.5f));

	PlayerCamera playerCamera;
	playerCamera.SetPixelsPerUnit(32);
	playerCamera.SetCameraTarget(&player.m_transform);

	refCameraManager.SetCurrentCamera(&playerCamera);

	const int MAP_LAYER = 0;
	const int PLAYER_LAYER = 1;

	// Make sure to document that the order of adding layers is important.
	scene->AddLayer(MAP_LAYER);
	scene->AddLayer(PLAYER_LAYER);

	Engine::TileMap testMap("testMap1.txt", 1);
	scene->AddTileMap(testMap, MAP_LAYER);
	// const size_t tileID = Engine::IDGenerator::CreateUUID();
	// Engine::Tile tile(tileID, Engine::STATIC, Engine::Vector2D<float>(1.0f, 12.0f), Engine::Vector2D<float>(2.0f, 1.0f), 1, true);
	// scene->Add(tileID, MAP_LAYER);

	scene->Add(playerID, PLAYER_LAYER);

	scene2->AddLayer(MAP_LAYER);
	scene2->AddLayer(PLAYER_LAYER);

	// Engine::TileMap testMap2("TestMap2.txt", 1);
	// scene2->AddTileMap(testMap2, MAP_LAYER);
	// cene2->Add(playerID, PLAYER_LAYER);

	refSceneManager.LoadScene("Level1");
	
	AppManagementEventHandlers appManagementEventHandlers(refEventManager, playerCamera);

	ptrAppInstance->Start();
	// Need to figure out how to change scenes, stop scenes, etc.

	return 0;
}