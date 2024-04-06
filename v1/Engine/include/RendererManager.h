#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "GameObjects/GameObject.h"
#include "CallbackSystem/CallbackSystem.h"

namespace Engine
{
	// RendererManager class. Manages the rendering of the application.
	class RendererManager
	{
	private:
		// SDL2 renderer pointer. This is the renderer that will be used to render all of the game objects.
		SDL_Renderer* renderer;

		// Pointer to the ICallbackSystem instance. Having a member variable that points
		// to a singleton is typically not a good practice,
		// but since the CallbackSystem is a basic messaging system, it is ok.
		ICallbackSystem* ptrICallbackSystem;
	
	public:
		RendererManager(SDL_Window* window);
		~RendererManager();

		SDL_Renderer* getRenderer() const;
		const SDL_Texture* loadTexture(const char* filePath);
		void clearScreen();
		void render(GameObject* gameObject, const int pixelsPerMeter);
		void display();
		void free();
	};
}