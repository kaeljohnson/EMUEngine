#pragma once

#include "../include/SDLWrapper/SDLWrapper.h"

#include "GameObjects/GameObject.h"
#include "CallbackSystem/CallbackSystem.h"

namespace Engine
{
	// RendererManager class. Manages the rendering of the application.
	class RendererManager
	{
	private:
		// SDL2 renderer pointer. This is the renderer that will be used to render all of the game objects.
		SDLRenderer* renderer;

		// Pointer to the ICallbackSystem instance. Having a member variable that points
		// to a singleton is typically not a good practice,
		// but since the CallbackSystem is a basic messaging system, it is ok.
		ICallbackSystem* ptrICallbackSystem;

		const int VIRTUAL_WIDTH;
		const int VIRTUAL_HEIGHT;

		const float SCALE_X;
		const float SCALE_Y;
		const float SCALE;

		int viewportWidth;
		int viewportHeight;
		int viewportX;
		int viewportY;
	
	public:
		RendererManager(SDLWindow* window, SDLDisplayMode& refDM);
		~RendererManager();

		SDLRenderer* getRenderer() const;
		const SDLTexture* loadTexture(const char* filePath);
		void clearScreen();
		void render(GameObject* gameObject, const int pixelsPerMeter);
		void display();
		void free();
	};
}