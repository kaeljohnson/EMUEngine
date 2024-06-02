#pragma once

#include <Engine.h>
#include "../include/EventListeners/AppManagementListener.h"
#include "../include/ClientObjects/ClientObject.h"

int main(int argc, char* args[])
{
	Engine::Init();

	CLIENT_INFO_D("Client Running!");

	Engine::ApplicationPtr ptrAppInstance = Engine::Application::GetInstance();

	Engine::ScenePtr scene = Engine::CreateScene();

	scene->SetSimulation(0.0f, 9.8f, 10);

	// Temp
	Engine::Texture tempTextureRed(255, 0, 0);
	Engine::Texture tempTextureBlue(0, 0, 265);

	Engine::PhysicsBodyPtr testBody =      Engine::CreatePhysicsBody(Engine::DYNAMIC, false, 63.5f, 3.0f, 1.0f, 2.0f);
	Engine::PhysicsBodyPtr testBody2 =     Engine::CreatePhysicsBody(Engine::DYNAMIC, false, 68.5f, 3.0f, 1.0f, 1.0f);
	Engine::PhysicsBodyPtr testBody3 =	   Engine::CreatePhysicsBody(Engine::DYNAMIC, false, 76.5f, 3.0f, 2.0f, 2.0f);
	Engine::PhysicsBodyPtr wallBody =      Engine::CreatePhysicsBody(Engine::STATIC, true, 15.0f, 35.0f, 2.0f, 80.0f);
	Engine::PhysicsBodyPtr wallRightBody = Engine::CreatePhysicsBody(Engine::STATIC, true, 100.0f, 35.0f, 2.0f, 80.0f);
	Engine::PhysicsBodyPtr groundBody =    Engine::CreatePhysicsBody(Engine::STATIC, true, 63.5f, 70.0f, 120.0f, 2.0f);
	
	Engine::PhysicsBodyPtr blockBody = Engine::CreatePhysicsBody(Engine::STATIC, false, 63.5f, 65.0f, 9.0f, 1.0f);
	Engine::PhysicsBodyPtr blockBody2 = Engine::CreatePhysicsBody(Engine::STATIC, false, 63.5f, 60.0f, 9.0f, 1.0f);
	Engine::PhysicsBodyPtr blockBody3 = Engine::CreatePhysicsBody(Engine::STATIC, false, 50.0f, 55.0f, 9.0f, 1.0f);
	
	Engine::Controllable player(testBody, tempTextureRed);

	ClientObject testGO2(testBody2, tempTextureRed);
	ClientObject testGO3(testBody3, tempTextureRed);
	ClientObject testWall(wallBody, tempTextureBlue);
	ClientObject testWallRight(wallRightBody, tempTextureBlue);
	ClientObject testGround(groundBody, tempTextureBlue);

	ClientObject testBlock(blockBody, tempTextureBlue);
	ClientObject testBlock2(blockBody2, tempTextureBlue);
	ClientObject testBlock3(blockBody3, tempTextureBlue);
	
	CLIENT_INFO_D(player.GetUUID());

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
	
	ptrAppInstance->PlayScene(scene);
	// Need to figure out how to change scenes, stop scenes, etc.

	return 0;
}