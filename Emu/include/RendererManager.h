#pragma once

#include "../include/SDLWrapper/SDLWrapper.h"

#include "Scenes/SceneObject.h"
#include "CallbackSystem/CallbackSystem.h"

namespace Engine
{
	// RendererManager class. Manages the rendering of the application.
	class RendererManager
	{
	private:
		// SDL2 renderer pointer. This is the renderer that will be used to render all of the scene objects.
		SDLRenderer* renderer;

		const int VIRTUAL_WIDTH;
		const int VIRTUAL_HEIGHT;

		float SCALE_X;
		float SCALE_Y;
		float SCALE;

		int viewportWidth;
		int viewportHeight;
		int viewportX;
		int viewportY;
	
	public:
		RendererManager(SDLWindow* window);
		~RendererManager();

		SDLRenderer* getRenderer() const;
		const SDLTexture* loadTexture(const char* filePath);
		void clearScreen();
		void render(SceneObject* sceneObject, const int pixelsPerMeter, const double interpolation);
		void setViewport(SDLWindow* ptrWindow);
		void display();
		void free();
	};
}