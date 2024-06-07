#pragma once

#include <Engine.h>
#include "../include/EventListeners/AppManagementListener.h"
#include "../include/ClientObjects/ClientObject.h"

int main(int argc, char* args[])
{
	Engine::Init();

	ENGINE_INFO_D("Client Running!");

	Engine::ApplicationPtr ptrAppInstance = Engine::Application::GetInstance();

	Engine::ScenePtr scene = Engine::CreateScene();

	scene->SetSimulation(0.0f, 9.8f, 10);

	// Temp
	Engine::TexturePtr tempTextureRed = Engine::CreateTexture(255, 0, 0);
	Engine::TexturePtr tempTextureBlue = Engine::CreateTexture(0, 0, 265);
	
	Engine::Controllable player(Engine::DYNAMIC, false, 63.5f, 3.0f, 1.0f, 2.0f, tempTextureRed);

	ClientObject testGO2(Engine::DYNAMIC, false, 68.5f, 3.0f, 1.0f, 1.0f, tempTextureRed);
	testGO2.GetPhysicsBody()->SetRestitution(1.0f);
	ClientObject testGO3(Engine::DYNAMIC, false, 76.5f, 3.0f, 2.0f, 2.0f, tempTextureRed);
	ClientObject testWall(Engine::STATIC, true, 15.0f, 35.0f, 2.0f, 80.0f, tempTextureBlue);
	ClientObject testWallRight(Engine::STATIC, true, 100.0f, 35.0f, 2.0f, 80.0f, tempTextureBlue);
	ClientObject testGround(Engine::STATIC, true, 63.5f, 70.0f, 120.0f, 2.0f, tempTextureBlue);

	ClientObject testBlock(Engine::STATIC, false, 63.5f, 65.0f, 9.0f, 1.0f, tempTextureBlue);
	ClientObject testBlock2(Engine::STATIC, false, 63.5f, 60.0f, 9.0f, 1.0f, tempTextureBlue);
	ClientObject testBlock3(Engine::STATIC, false, 43.0f, 55.0f, 9.0f, 1.0f, tempTextureBlue);
	ClientObject testBlock4(Engine::STATIC, false, 50.0f, 50.0f, 9.0f, 1.0f, tempTextureBlue);
	ClientObject testBlock5(Engine::STATIC, false, 50.0f, 45.0f, 9.0f, 1.0f, tempTextureBlue);
	ClientObject testBlock6(Engine::STATIC, false, 50.0f, 40.0f, 9.0f, 1.0f, tempTextureBlue);
	ClientObject testBlock7(Engine::STATIC, false, 50.0f, 35.0f, 9.0f, 1.0f, tempTextureBlue);
	ClientObject testBlock8(Engine::STATIC, false, 70.0f, 35.0f, 1.0f, 1.0f, tempTextureBlue);
	ClientObject testBlock9(Engine::STATIC, false, 74.0f, 30.0f, 1.0f, 1.0f, tempTextureBlue);
	ClientObject testBlock10(Engine::STATIC, false, 78.0f, 25.0f, 1.0f, 1.0f, tempTextureBlue);
	ClientObject testBlock11(Engine::STATIC, false, 35.0f, 65.0f, 1.0f, 1.0f, tempTextureBlue);
	ClientObject testBlock12(Engine::STATIC, false, 40.0f, 60.0f, 1.0f, 1.0f, tempTextureBlue);
	ClientObject testBlock13(Engine::STATIC, false, 35.0f, 55.0f, 1.0f, 1.0f, tempTextureBlue);
	ClientObject testBlock14(Engine::STATIC, false, 40.0f, 50.0f, 1.0f, 1.0f, tempTextureBlue);
	ClientObject testBlock15(Engine::STATIC, false, 35.0f, 45.0f, 1.0f, 1.0f, tempTextureBlue);
	ClientObject testBlock16(Engine::STATIC, false, 40.0f, 40.0f, 1.0f, 1.0f, tempTextureBlue);
	ClientObject testBlock17(Engine::STATIC, false, 35.0f, 35.0f, 1.0f, 1.0f, tempTextureBlue);
	ClientObject testBlock18(Engine::STATIC, false, 40.0f, 30.0f, 1.0f, 1.0f, tempTextureBlue);
	ClientObject testBlock19(Engine::STATIC, false, 35.0f, 25.0f, 1.0f, 1.0f, tempTextureBlue);
	ClientObject testBlock20(Engine::STATIC, false, 40.0f, 20.0f, 1.0f, 1.0f, tempTextureBlue);
	ClientObject testBlock21(Engine::STATIC, false, 35.0f, 15.0f, 1.0f, 1.0f, tempTextureBlue);
	ClientObject testBlock22(Engine::STATIC, false, 40.0f, 10.0f, 1.0f, 1.0f, tempTextureBlue);
	
	ENGINE_INFO_D(player.GetUUID());

	AppManagementListener appManagementListener("App management listener");

	ptrAppInstance->CreateEventListener(appManagementListener);

	scene->AddEventListener(player);

	scene->Add(player);
	scene->Add(testGO2);
	scene->Add(testGO3);
	scene->Add(testWall);
	scene->Add(testGround);
	scene->Add(testWallRight);
	scene->Add(testBlock);
	scene->Add(testBlock2);
	scene->Add(testBlock3);
	scene->Add(testBlock4);
	scene->Add(testBlock5);
	scene->Add(testBlock6);
	scene->Add(testBlock7);
	scene->Add(testBlock8);
	scene->Add(testBlock9);
	scene->Add(testBlock10);
	scene->Add(testBlock11);
	scene->Add(testBlock12);
	scene->Add(testBlock13);
	scene->Add(testBlock14);
	scene->Add(testBlock15);
	scene->Add(testBlock16);
	scene->Add(testBlock17);
	scene->Add(testBlock18);
	scene->Add(testBlock19);
	scene->Add(testBlock20);
	scene->Add(testBlock21);
	scene->Add(testBlock22);

	
	ptrAppInstance->PlayScene(scene);
	// Need to figure out how to change scenes, stop scenes, etc.

	return 0;
}