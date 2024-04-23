#pragma once

#include <Engine.h>
#include <memory>
#include "../include/Layers/AppManagementLayer.h"

int main(int argc, char* args[])
{
	Engine::Init();

	CLIENT_INFO_D("Client Running!");

	Engine::Application* ptrAppInstance = Engine::Application::GetInstance();

	// Need to figure out who is responsible for deleting the scene.
	std::shared_ptr<Engine::Scene> scene = ptrAppInstance->CreateScene("Test Scene");

	ptrAppInstance->SetTimeStep(1.0f / 60.0f);
	ptrAppInstance->SetPixelsPerMeter(10);
	scene->SetSimulation(0.0f, 4.9f);

	// Temp
	Engine::Texture tempTextureRed(255, 0, 0);
	Engine::Texture tempTextureBlue(0, 0, 265);

	// Maybe The physics body interface just automatically attaches the body to the world?
	Engine::IPhysicsBody* testBody =      Engine::CreatePhysicsBody(Engine::DYNAMIC, 63.5f, 3.0f, 1.0f, 1.0f, 1.0f, 10.0f, 30.0, 2.0, 2.0, true, false, true);
	Engine::IPhysicsBody* testBody2 =     Engine::CreatePhysicsBody(Engine::DYNAMIC, 68.5f, 3.0f, 1.0f, 1.0f, 1.0f, 10.0f, 30.0, 2.0, 2.0, true, false, true);
	Engine::IPhysicsBody* testBody3 =	  Engine::CreatePhysicsBody(Engine::DYNAMIC, 76.5f, 3.0f, 8.0f, 8.0f, 1.0f, 10.0f, 30.0, 2.0, 2.0, true, false, true);
	Engine::IPhysicsBody* wallBody =      Engine::CreatePhysicsBody(Engine::STATIC, 15.0f, 35.0f, 2.0f, 80.0f, 0.0f, 50.0f, 0.0, 0.0, 0.0, true, true, true);
	Engine::IPhysicsBody* wallRightBody = Engine::CreatePhysicsBody(Engine::STATIC, 100.0f, 35.0f, 2.0f, 80.0f, 0.0f, 50.0f, 0.0, 0.0, 0.0, true, true, true);
	Engine::IPhysicsBody* groundBody =    Engine::CreatePhysicsBody(Engine::STATIC, 63.5f, 70.0f, 120.0f, 2.0f, 0.0f, 50.0f, 0.0, 1.0, 1.0, true, true, true);
	
	// SceneObject to be renamed and have a constructor that only takes
	// a texture. Making the object exist outside the physics world.
	Engine::SceneObject testGO(testBody, &tempTextureRed);
	Engine::SceneObject testGO2(testBody2, &tempTextureRed);
	Engine::SceneObject testGO3(testBody3, &tempTextureRed);
	Engine::SceneObject testWall(wallBody, &tempTextureBlue);
	Engine::SceneObject testWallRight(wallRightBody, &tempTextureBlue);
	Engine::SceneObject testGround(groundBody, &tempTextureBlue);
	
	AppManagementLayer appLayer("App management layer");
	Engine::Layer simulationLayer("Test Layer 1");

	// Note in documentation that the client must keep in mind
	// which order they want layers processed and rendered in.
	scene->PushLayer(&appLayer);
	scene->PushLayer(&simulationLayer);

	simulationLayer.AddSceneObject(&testWall);
	simulationLayer.AddSceneObject(&testGround);
	simulationLayer.AddSceneObject(&testGO);
	simulationLayer.AddSceneObject(&testGO2);
	simulationLayer.AddSceneObject(&testGO3);
	simulationLayer.AddSceneObject(&testWallRight);
	
	ptrAppInstance->Run();

	return 0;
}