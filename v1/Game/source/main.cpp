#pragma once

#include <stdio.h>

#include "../include/EngineIncludes.h"

const char* APPLICATION_NAME = "Wizard Game v1";\

// Main function. Starting point for all C++ programs.
int main(int argc, char* args[])
{
	// Call to Init function. Initializes the logger, the SDL2 and SDL2_image libraries.
	Engine::Init();
	// Call to Game constructor. Creates or "instantiates" and object of type "Game" named game.
	Engine::Application app(APPLICATION_NAME);
	// Log a warning if app was instantiated without initializing the engine.

	CLIENT_TRACE("Client Running!");

	Engine::GameObject go(20, 20, true, true);

	app.addToLayerStack(go);
	
	app.run();

	app.end();

	return 0;
}