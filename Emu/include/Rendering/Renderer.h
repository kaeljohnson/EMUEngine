#pragma once

#include "../SDLWrapper/SDLWrapper.h"
#include "IRenderer.h"
#include "../Scenes/SceneObject.h"

namespace Engine
{
	class Renderer : public IRenderer
	{
	public:
		Renderer(SDLWindow* renderer);
		~Renderer();

		void SetViewport(SDLWindow* ptrWindow);
		void ClearScreen();
		void Render(SceneObject* sceneObject, const int pixelsPerMeter, const double interpolation);
		void Display();

		void free();

		SDLRenderer* renderer;
	private:
		

		const int VIRTUAL_WIDTH;
		const int VIRTUAL_HEIGHT;

		float SCALE_X;
		float SCALE_Y;
		float SCALE;

		int viewportWidth;
		int viewportHeight;
		int viewportX;
		int viewportY;

		bool rendererCreated;
	};
}