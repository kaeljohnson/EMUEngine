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

	Engine::PhysicsBodyPtr testBody =      Engine::CreatePhysicsBody(Engine::DYNAMIC, 63.5f, 3.0f, 1.0f, 1.0f, 1.0f, 10.0f, 3.0, 1.0, 1.0, true, false, true);
	Engine::PhysicsBodyPtr testBody2 =     Engine::CreatePhysicsBody(Engine::DYNAMIC, 68.5f, 3.0f, 1.0f, 1.0f, 1.0f, 10.0f, 0.0, 1.0, 1.0, true, false, true);
	Engine::PhysicsBodyPtr testBody3 =	   Engine::CreatePhysicsBody(Engine::DYNAMIC, 76.5f, 3.0f, 2.0f, 2.0f, 1.0f, 10.0f, 0.0, 1.0, 1.0, true, false, true);
	Engine::PhysicsBodyPtr wallBody =      Engine::CreatePhysicsBody(Engine::STATIC, 15.0f, 35.0f, 2.0f, 80.0f, 0.0f, 50.0f, 0.0, 0.0, 0.0, true, true, true);
	Engine::PhysicsBodyPtr wallRightBody = Engine::CreatePhysicsBody(Engine::STATIC, 100.0f, 35.0f, 2.0f, 80.0f, 0.0f, 50.0f, 0.0, 0.0, 0.0, true, true, true);
	Engine::PhysicsBodyPtr groundBody =    Engine::CreatePhysicsBody(Engine::STATIC, 63.5f, 70.0f, 120.0f, 2.0f, 0.0f, 50.0f, 0.0, 1.0, 1.0, true, true, true);
	
	ClientObject testGO(testBody, &tempTextureRed);
	ClientObject testGO2(testBody2, &tempTextureRed);
	ClientObject testGO3(testBody3, &tempTextureRed);
	ClientObject testWall(wallBody, &tempTextureBlue);
	ClientObject testWallRight(wallRightBody, &tempTextureBlue);
	ClientObject testGround(groundBody, &tempTextureBlue);
	
	AppManagementListener appManagementListener("App management listener");

	ptrAppInstance->AddEventListener(&appManagementListener);

	scene->Add(&testWall);
	scene->Add(&testGround);
	scene->Add(&testGO);
	scene->Add(&testGO2);
	scene->Add(&testGO3);
	scene->Add(&testWallRight);
	
	ptrAppInstance->PlayScene(scene);
	// Need to figure out how to change scenes, stop scenes, etc.

	return 0;
}