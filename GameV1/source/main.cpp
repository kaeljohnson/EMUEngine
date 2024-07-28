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

	std::unordered_map<Engine::EventType, Engine::EventHandler>& refEventHandlerMap = ptrAppInstance->GetEventHandlers();
	AppManagementEventHandlers appManagementEventHandlers(refEventHandlerMap);

	Engine::ScenePtr scene = Engine::CreateScene();

	// Need physcis to scale with pixels per meter.
	scene->SetSimulation(0.0f, 3.1f, 32);

	// Temp
	Engine::TexturePtr tempTextureRed = Engine::CreateTexture(0, 0, 0);
	
	Player player(1.0f, 1.0f, 0.75f, 0.75f, tempTextureRed);

	Engine::ScrollingCamera scrollCamera;
	scrollCamera.SetScrollingSpeeds(Engine::Vector2D<float>(0.0005f, 0.0f));
	scrollCamera.SetCameraPosition(Engine::Vector2D<float>(0.0f, 38.5f));


	PlayerCamera playerCamera;
	playerCamera.SetCameraTarget(&player);


	Engine::CameraManager& refCameraManager = ptrAppInstance->GetCameraManager();

	refCameraManager.SetCurrentCamera(&playerCamera);
	// refCameraManager.SetCurrentCamera(&scrollCamera);


	CLIENT_INFO_D("Player UUID: " + player.GetUUID());

	Engine::TileMap testMap("TestMap1.txt", 1);

	const int MAP_LAYER = 0;
	const int PLAYER_LAYER = 1;

	// Make sure to document that the order of adding layers is important.
	scene->AddLayer(MAP_LAYER);
	scene->AddLayer(PLAYER_LAYER);
	
	scene->AddTileMap(testMap, MAP_LAYER);

	scene->Add(player, PLAYER_LAYER);

	// AppManagementListener appManagementListener("App management listener");

	// ptrAppInstance->CreateEventListener(appManagementListener);
	
	ptrAppInstance->PlayScene(scene);
	// Need to figure out how to change scenes, stop scenes, etc.

	return 0;
}