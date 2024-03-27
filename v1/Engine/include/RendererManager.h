#pragma once

#include <SDL.h>													// External includes.
#include <SDL_image.h>

#include "GameObjects/GameObject.h"
#include "Events/IEventAction.h"


namespace Engine
{
	// RendererManager class. Manages the rendering of the application.
	class RendererManager												
	{
	private:
		// SDL2 renderer pointer. This is the renderer that will be used to render all of the game objects.
		SDL_Renderer* renderer;

		IEventAction* ptrEventActionInterface;
	public:
		// RendererManager constructor.
		RendererManager(SDL_Window* window, IEventAction* eventActionInterface);
		~RendererManager();

		// RendererManager functions.
		SDL_Renderer* getRenderer() const;
		const SDL_Texture* loadTexture(const char* filePath);
		void clearScreen();
		void render(GameObject* gameObject);
		void display();
		void free();
	};
}