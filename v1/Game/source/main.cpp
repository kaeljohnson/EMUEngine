#pragma once

#include <stdio.h>

#include "../include/EngineIncludes.h"
#include "../include/WindowManagerLayer.h"

const char* APPLICATION_NAME = "Wizard Game v1";\

// Main function. Starting point for all C++ programs.
int main(int argc, char* args[])
{
	// Once layers are pushed to the applications layer stack, the application takes the
	// ownership of the layer and therefore the responsibility of deleting the layers.
	// If the client pops the layer from the stack, the client is responsible for 
	// deleting the layer.
	
	// Call to Init function. Initializes the logger, the SDL2 and SDL2_image libraries.
	Engine::Init();
	// Call to Application constructor. Creates or "instantiates" and object of type "Application" named app./
	Engine::Application app(APPLICATION_NAME);

	// Probably should not be able to do this. Figure out a better way to handle this.
	// Could make app a singleton and have a getter for anything needed in the window manager. 
	// Instead of letting the client have the whole thing.
	
	
	Engine::WindowManager* ptrWindowManager = app.getWindowManager();

	Engine::WindowManagerLayer windowManagerLayer(app);
	app.addToLayerStack(&windowManagerLayer);

	CLIENT_INFO("Client Running!");

	Engine::GameObject go(20, 20, true, true);
	Engine::Layer testLayer("Test Layer 1");
	Engine::Layer testLayer2("Test Layer 2");
	Engine::Layer testLayer3("Test Layer 3");

	CLIENT_TRACE("STOP 1");
	app.addToLayerStack(&testLayer); 
	app.addToLayerStack(&testLayer2);
	app.addToLayerStack(&testLayer3);

	CLIENT_TRACE("STOP 2");
	
	app.run();

	// End client application. Engine cleanup handled in Engine app class.	
	// end();

	return 0;
}