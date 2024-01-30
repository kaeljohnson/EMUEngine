#pragma once

#include <stdio.h>

#include "../include/EngineIncludes.h"														    /// Need engine code

const char* APPLICATION_NAME = "Wizard Game v1";

void test()
{
	printf("Test user defined event callback.\n");
}


int main(int argc, char* args[])																// Main function. Starting point for all C++ programs.
{

	Engine::Init;

	Engine::Application app(APPLICATION_NAME);													// Call to Game constructor. Creates or "instantiates" and object of type "Game" named game.
	// Log a warning if app was instantiated without initializing the engine.

	Engine::EventHandlers& eventHandlers = app.getEventHandlers();

	eventHandlers.aKeyDown = test;

	app.run();																				    // Call start function on newly created Game object. This starts the game loop.

	app.end();

	return 0;																					// main function is declared as int so it must return an int.
} 