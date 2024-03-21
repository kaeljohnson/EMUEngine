#pragma once

#include "../include/EngineIncludes.h"

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

	// Call to Application constructor. Instantiates an object of type "Application" named app.
	Engine::Application app(APPLICATION_NAME);
	
	// Get the event system interface from the application.
	Engine::IEventAction* eventActionInterface = app.getEventActionInterface();

	CLIENT_INFO("Client Running!");

	Engine::GameObject go = { 20, 20, 20, 20, true, true, true };

	Engine::Layer testLayer("Test Layer 1");
	Engine::Layer testLayer2("Test Layer 2");

	CLIENT_TRACE("STOP 1");

	app.pushToLayerStack(&testLayer);
	app.pushToLayerStack(&testLayer2);
	
	app.popLayerFromStack(&testLayer);

	CLIENT_TRACE("STOP 2");
	
	app.run();

	// End client application. Engine cleanup handled in Engine app class.
	// end();

	return 0;
}