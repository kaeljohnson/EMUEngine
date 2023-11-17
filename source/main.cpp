#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include "../headers/RendererManager.h"
#include "../headers/WindowManager.h"
#include "../headers/Game.h"

const int SCREEN_WIDTH = 1800;
const int SCREEN_HEIGHT = 900;

void initializeSDLFeatures()
{
	if (!IMG_Init(IMG_INIT_PNG))
		printf("IMG Init failed! SDL_Error: %s\n", SDL_GetError());

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		printf("SDL Init failed! SDL_Error: %s\n", SDL_GetError());
}

int main(int argc, char* args[])
{
	initializeSDLFeatures();

	WindowManager windowManager("Nightmare Game V1", SCREEN_WIDTH, SCREEN_HEIGHT);
	RendererManager rendererManager(windowManager.getWindow());

	Game game(rendererManager);

	game.start();

	rendererManager.free();
	windowManager.free();

	return 0;
}