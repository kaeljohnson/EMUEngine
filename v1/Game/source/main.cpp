#pragma once

#include "../include/EngineIncludes.h"
#include "../include/Layers/AppManagementLayer.h"

const char* APPLICATION_NAME = "Wizard Game v1";

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

	// Call to Application constructor. Instantiates an object of type "Application" named app.
	Engine::Application app(APPLICATION_NAME);

	AppManagementLayer appLayer(&app.CallbackSystem);
	Engine::Layer testLayer("Test Layer 1", &app.CallbackSystem);
	Engine::Layer testLayer2("Test Layer 2", &app.CallbackSystem);

	// Temp
	SDL_Surface* surface = SDL_CreateRGBSurface(0, 1000, 1000, 32, 0, 0, 0, 0);
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 0, 0));
	SDL_Texture* tempTextureRed = SDL_CreateTextureFromSurface(app.getRenderer(), surface);
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 0));
	SDL_Texture* tempTextureBlue = SDL_CreateTextureFromSurface(app.getRenderer(), surface);
	SDL_FreeSurface(surface);

	Engine::GameObject testGO(Engine::DYNAMIC, 115.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, true, false, true, tempTextureRed);
	Engine::GameObject testGround(Engine::STATIC, 115.0f, 120.0f, 120.0f, 2.0f, 0.0f, 0.0f, true, true, true, tempTextureBlue);

	testLayer.addGameObject(&testGround);
	testLayer.addGameObject(&testGO);
	testLayer.removeGameObject(&testGround);

	CLIENT_TRACE("STOP 1");
	app.pushToLayerStack(&appLayer);
	app.pushToLayerStack(&testLayer);
	app.pushToLayerStack(&testLayer2);
	
	app.popLayerFromStack(&testLayer2);

	testLayer.addGameObject(&testGround);

	CLIENT_TRACE("STOP 2");
	
	app.run();

	// End client application. Engine cleanup handled in Engine app class.
	// end();

	return 0;
}