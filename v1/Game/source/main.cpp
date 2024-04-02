#pragma once

#include "../include/EngineIncludes.h"
#include "../include/Layers/AppManagementLayer.h"

// Main function. Starting point for all C++ programs.
int main(int argc, char* args[])
{
	// Once layers are pushed to the applications layer stack, the application takes the
	// ownership of the layer and therefore the responsibility of deleting the layers.
	// If the client pops the layer from the stack, the client is responsible for 
	// deleting the layer.
	
	// Call to Init function. Initializes the logger, the SDL2 and SDL2_image libraries.
	Engine::Init();

	CLIENT_INFO("Client Running!");

	// Keep instance on heap? Probably don't want this to be a singleton.
	Engine::Application* appInstance = Engine::Application::getInstance();

	// app->createSimulation(0.0f, 9.8f, 1.0f / 60.0f, 6, 2);

	AppManagementLayer appLayer("App management layer");
	Engine::Layer testLayer("Test Layer 1");
	Engine::Layer testLayer2("Test Layer 2");

	// Temp
	Engine::Texture tempTextureRed(255, 0, 0);
	Engine::Texture tempTextureBlue(0, 0, 255);

	Engine::GameObject testGO(Engine::DYNAMIC, 115.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, true, false, true, &tempTextureRed);
	Engine::GameObject testGround(Engine::STATIC, 115.0f, 120.0f, 120.0f, 2.0f, 0.0f, 0.0f, true, true, true, &tempTextureBlue);

	testLayer.addGameObject(&testGround);
	testLayer.addGameObject(&testGO);

	appInstance->pushToLayerStack(&appLayer);
	appInstance->pushToLayerStack(&testLayer);
	
	appInstance->run();


	delete appInstance;
	appInstance = nullptr;

	return 0;
}