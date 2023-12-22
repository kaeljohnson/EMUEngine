#pragma once

#include <SDL.h>																				////////////
#include <SDL_image.h>																			//	External library Includes.
#include <stdio.h>																				////////////

#include "../headers/RendererManager.h"															////////////
#include "../headers/WindowManager.h"															//  Header file includes.
#include "../headers/Game.h"																	////////////

const int SCREEN_WIDTH = 1800;																	// Set screen width. All caps to indicate this is a global variable. NOT a best practice, temporary solution.
const int SCREEN_HEIGHT = 900;																	// Set screen height. Same reasoning as width.

void initializeSDLFeatures()																	// Define function for initializing SDL features. Note: void function, nothing to return.
{
	if (!IMG_Init(IMG_INIT_PNG))																// Call to SDL "IMG_Init" function.
		printf("IMG Init failed! SDL_Error: %s\n", SDL_GetError());								// If "IMG_Init" function fails, print the last SDL error to the console. We will need more robust error logging later.

	if (SDL_Init(SDL_INIT_VIDEO) < 0)															// Call to SDL "SDL_Init" function.
		printf("SDL Init failed! SDL_Error: %s\n", SDL_GetError());								// If "SDL_Init" fails, print last SDL error to the console.
}

int main(int argc, char* args[])																// Main function. Starting point for all C++ programs.
{
	initializeSDLFeatures();																	// Call to initializeSDLFeatures function. A function "call" means "run this function". Since "initializeSDLFeatures" is a void function, there is no return value to assign to a variable.

	WindowManager windowManager("Wizard Game V1", SCREEN_WIDTH, SCREEN_HEIGHT);					// Call to WindowManager constructor. Creates or "instantiates" an object of type "WindowManager" named windowManager. The WindowManager class takes 3 arguments: 'std::string', 'const int', 'const int'.
	RendererManager rendererManager(windowManager.getWindow());									// Call to RendererManager constructor. Creates or "instantiates" an object of type "RendererManager" named rendererManager. The RendererManager class takes 1 argument: 'SDL_window*'.

	Game game(rendererManager);																	// Call to Game constructor. Creates or "instantiates" and object of type "Game" named game.

	game.start();																				// Call start function on newly created Game object. This starts the game loop.

	rendererManager.free();																		// Call free function on RendererManager object. This function will free any dynamically allocated memory within our RendererManager object.
	windowManager.free();																		// Call free function on WindowManager object. This function will free any dynamically allocated memory within our WindowManager object.

	return 0;																					// main function is declared as int so it must return an int.
}