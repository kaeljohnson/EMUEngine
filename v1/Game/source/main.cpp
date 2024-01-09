#pragma once

#include "../include/EngineIncludes.h"														    /// Need engine code

const int SCREEN_WIDTH = 1800;																	// Set screen width. All caps to indicate this is a global variable. NOT a best practice, temporary solution.
const int SCREEN_HEIGHT = 900;																	// Set screen height. Same reasoning as width.

int main(int argc, char* args[])																// Main function. Starting point for all C++ programs.
{

	Engine::WindowManager windowManager("Wizard Game V1", SCREEN_WIDTH, SCREEN_HEIGHT);			// Call to WindowManager constructor. Creates or "instantiates" an object of type "WindowManager" named windowManager. The WindowManager class takes 3 arguments: 'std::string', 'const int', 'const int'.
	Engine::RendererManager rendererManager(windowManager.getWindow());							// Call to RendererManager constructor. Creates or "instantiates" an object of type "RendererManager" named rendererManager. The RendererManager class takes 1 argument: 'SDL_window*'.

	Engine::Application app(rendererManager);															// Call to Game constructor. Creates or "instantiates" and object of type "Game" named game.

	app.start();																				// Call start function on newly created Game object. This starts the game loop.

	rendererManager.free();																		// Call free function on RendererManager object. This function will free any dynamically allocated memory within our RendererManager object.
	windowManager.free();																		// Call free function on WindowManager object. This function will free any dynamically allocated memory within our WindowManager object.

	return 0;																					// main function is declared as int so it must return an int.
}