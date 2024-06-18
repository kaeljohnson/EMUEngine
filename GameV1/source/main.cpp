#pragma once

#include <Engine.h>
#include "../include/EventListeners/AppManagementListener.h"

int main(int argc, char* args[])
{
	Engine::Init();

	CLIENT_INFO_D("Client Running!");

	Engine::ApplicationPtr ptrAppInstance = Engine::Application::GetInstance();

	Engine::ScenePtr scene = Engine::CreateScene();

	// Need physcis to scale with pixels per meter.
	scene->SetSimulation(0.0f, 3.1f, 32.0f);

	// Temp
	Engine::TexturePtr tempTextureRed = Engine::CreateTexture(255, 0, 0);
	Engine::TexturePtr tempTextureBlue = Engine::CreateTexture(0, 0, 265);
	
	Engine::Controllable player(4.0f, 20.0f, 0.9f, 0.9f, tempTextureRed);

	Engine::TileMap testMap("TestMap1.txt", 1);

	scene->AddMap(testMap);
	
	CLIENT_INFO_D(player.GetUUID());

	AppManagementListener appManagementListener("App management listener");

	ptrAppInstance->CreateEventListener(appManagementListener);

	scene->AddEventListener(player);

	scene->Add(player);
	
	ptrAppInstance->PlayScene(scene);
	// Need to figure out how to change scenes, stop scenes, etc.

	return 0;
}