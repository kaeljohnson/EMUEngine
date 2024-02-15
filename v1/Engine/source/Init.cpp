#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include "../include/Init.h"

namespace Engine
{
	Init::Init()
	{
		/*
			Call to SDL "IMG_Init" and "SDL_Init" functions.
			If these function fail, print the last 
			SDL error to the console. We will need more robust error logging later.
		*/

		if (!IMG_Init(IMG_INIT_PNG))
			printf("IMG Init failed! SDL_Error: %s\n", SDL_GetError());

		if (SDL_Init(SDL_INIT_VIDEO) < 0)
			printf("SDL Init failed! SDL_Error: %s\n", SDL_GetError());
	}
}
