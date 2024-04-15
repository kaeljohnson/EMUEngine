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
	Engine::IPhysicsBody* testBody = Engine::CreatePhysicsBody(Engine::DYNAMIC, 63.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0, 0.5, true, false, true);
	Engine::IPhysicsBody* groundBody = Engine::CreatePhysicsBody(Engine::STATIC, 63.5f, 70.0f, 120.0f, 2.0f, 0.0f, 0.0f, 0.0, 0.0, true, true, true);

	// GameObject to be renamed and have a constructor that only takes
	// a texture. Making the object exist outside the physics world.
	Engine::GameObject testGO(testBody, &tempTextureRed);
	Engine::GameObject testGround(groundBody, &tempTextureBlue);

	AppManagementLayer appLayer("App management layer");
	Engine::Layer testLayer("Test Layer 1");

	testLayer.AddGameObject(&testGround);
	testLayer.AddGameObject(&testGO);

	ptrAppInstance->PushToLayerStack(&appLayer);
	ptrAppInstance->PushToLayerStack(&testLayer);

	// Note in documentation that the client must keep in mind
	// which order they want layers processed and rendered in.
	ptrAppInstance->PopLayerFromStack(&appLayer);
	ptrAppInstance->PushToLayerStack(&appLayer);
	
	ptrAppInstance->Run();

	delete ptrAppInstance;
	ptrAppInstance = nullptr;

	return 0;
}