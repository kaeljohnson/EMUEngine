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

	AppManagementLayer appLayer(&appInstance->CallbackSystem);
	Engine::Layer testLayer("Test Layer 1", &appInstance->CallbackSystem);
	Engine::Layer testLayer2("Test Layer 2", &appInstance->CallbackSystem);

	// Temp
	SDL_Surface* surface = SDL_CreateRGBSurface(0, 1000, 1000, 32, 0, 0, 0, 0);
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 0, 0));
	SDL_Texture* tempTextureRed = SDL_CreateTextureFromSurface(appInstance->getRenderer(), surface);
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 0));
	SDL_Texture* tempTextureBlue = SDL_CreateTextureFromSurface(appInstance->getRenderer(), surface);  
	SDL_FreeSurface(surface);

	Engine::GameObject testGO(Engine::DYNAMIC, 115.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, true, false, true, tempTextureRed);
	Engine::GameObject testGround(Engine::STATIC, 115.0f, 120.0f, 120.0f, 2.0f, 0.0f, 0.0f, true, true, true, tempTextureBlue);

	testLayer.addGameObject(&testGround);
	testLayer.addGameObject(&testGO);
	testLayer.removeGameObject(&testGround);

	CLIENT_TRACE("STOP 1");
	appInstance->pushToLayerStack(&appLayer);
	appInstance->pushToLayerStack(&testLayer);
	appInstance->pushToLayerStack(&testLayer2);
	
	appInstance->popLayerFromStack(&testLayer2);

	testLayer.addGameObject(&testGround);

	CLIENT_TRACE("STOP 2");
	
	appInstance->run();

	// End client application. Engine cleanup handled in Engine app class.
	// end();
	// delete appLayer;
	// delete testLayer;
	// delete testLayer2;
	delete appInstance;
	appInstance = nullptr;

	return 0;
}