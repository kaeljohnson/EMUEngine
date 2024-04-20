#pragma once

#include <Engine.h>
#include "../include/Layers/AppManagementLayer.h"

int main(int argc, char* args[])
{
	Engine::Init();

	CLIENT_INFO("Client Running!");

	Engine::Application* ptrAppInstance = Engine::Application::GetInstance();

	ptrAppInstance->SetSimulation(0.0f, 9.8f, 1.0f / 60.0f, 10);

	// Temp
	Engine::Texture tempTextureRed(255, 0, 0);
	Engine::Texture tempTextureBlue(0, 0, 265);

	// Maybe The physics body interface just automatically attaches the body to the world?
	Engine::IPhysicsBody* testBody =      Engine::CreatePhysicsBody(Engine::DYNAMIC, 63.5f, 3.0f, 1.0f, 1.0f, 1.0f, 10.0f, 30.0, 1.0, 1.0, true, false, true);
	Engine::IPhysicsBody* testBody2 =     Engine::CreatePhysicsBody(Engine::DYNAMIC, 68.5f, 3.0f, 1.0f, 1.0f, 1.0f, 10.0f, 30.0, 1.0, 1.0, true, false, true);
	Engine::IPhysicsBody* testBody3 =	  Engine::CreatePhysicsBody(Engine::DYNAMIC, 76.5f, 3.0f, 8.0f, 8.0f, 1.0f, 10.0f, 30.0, 1.0, 1.0, true, false, true);
	Engine::IPhysicsBody* wallBody =      Engine::CreatePhysicsBody(Engine::STATIC, 15.0f, 35.0f, 2.0f, 80.0f, 0.0f, 50.0f, 0.0, 0.0, 0.0, true, true, true);
	Engine::IPhysicsBody* wallRightBody = Engine::CreatePhysicsBody(Engine::STATIC, 100.0f, 35.0f, 2.0f, 80.0f, 0.0f, 50.0f, 0.0, 0.0, 0.0, true, true, true);
	Engine::IPhysicsBody* groundBody =    Engine::CreatePhysicsBody(Engine::STATIC, 63.5f, 70.0f, 120.0f, 2.0f, 0.0f, 50.0f, 0.0, 1.0, 1.0, true, true, true);
	
	// GameObject to be renamed and have a constructor that only takes
	// a texture. Making the object exist outside the physics world.
	Engine::GameObject testGO(testBody, &tempTextureRed);
	Engine::GameObject testGO2(testBody2, &tempTextureRed);
	Engine::GameObject testGO3(testBody3, &tempTextureRed);
	Engine::GameObject testWall(wallBody, &tempTextureBlue);
	Engine::GameObject testWallRight(wallRightBody, &tempTextureBlue);
	Engine::GameObject testGround(groundBody, &tempTextureBlue);
	

	AppManagementLayer appLayer("App management layer");
	Engine::Layer simulationLayer("Test Layer 1");

	//simulationLayer.AddGameObject(&testGO3);

	// Note in documentation that the client must keep in mind
	// which order they want layers processed and rendered in.
	ptrAppInstance->PushToLayerStack(&appLayer);
	ptrAppInstance->PushToLayerStack(&simulationLayer);

	simulationLayer.AddGameObject(&testWall);
	simulationLayer.AddGameObject(&testGround);
	simulationLayer.AddGameObject(&testGO);
	simulationLayer.AddGameObject(&testGO3);
	simulationLayer.AddGameObject(&testWallRight);
	simulationLayer.AddGameObject(&testGO2);

	// Removing a game object from a layer does not delete the object.
	simulationLayer.RemoveGameObject(&testGO2);
	simulationLayer.AddGameObject(&testGO2);
	
	ptrAppInstance->Run();



	return 0;
}