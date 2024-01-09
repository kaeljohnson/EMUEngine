#pragma once

#include "../include/EngineIncludes.h"														    /// Need engine code

const int SCREEN_WIDTH = 1800;																	// Set screen width. All caps to indicate this is a global variable. NOT a best practice, temporary solution.
const int SCREEN_HEIGHT = 900;																	// Set screen height. Same reasoning as width.
const char* APPLICATION_NAME = "Wizard Game v1";

int main(int argc, char* args[])																// Main function. Starting point for all C++ programs.
{

	Engine::Init;

	Engine::Application app(APPLICATION_NAME, SCREEN_WIDTH, SCREEN_HEIGHT);													// Call to Game constructor. Creates or "instantiates" and object of type "Game" named game.

	app.run();																				// Call start function on newly created Game object. This starts the game loop.

	app.end();

	return 0;																					// main function is declared as int so it must return an int.
} 