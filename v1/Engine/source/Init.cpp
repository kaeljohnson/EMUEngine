#include <SDL.h>													// External includes.
#include <SDL_image.h>
#include <stdio.h>

#include "../include/Init.h"

namespace Engine
{
	Init::Init()
	{
		if (!IMG_Init(IMG_INIT_PNG))													// Call to SDL "IMG_Init" function.
			printf("IMG Init failed! SDL_Error: %s\n", SDL_GetError());					// If "IMG_Init" function fails, print the last SDL error to the console. We will need more robust error logging later.

		if (SDL_Init(SDL_INIT_VIDEO) < 0)												// Call to SDL "SDL_Init" function.
			printf("SDL Init failed! SDL_Error: %s\n", SDL_GetError());					// If "SDL_Init" fails, print last SDL error to the console.
	}
}
