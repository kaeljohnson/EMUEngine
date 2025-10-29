#pragma once

#include "../ECS/ECS.h"
#include "../Includes.h"
#include "../Components.h"
#include "../AssetManager.h"

namespace Engine
{
	class IRenderer
	{
	public:
		IRenderer(ECS& refECS, AssetManager& refAssetManager);
		~IRenderer();

		/*
			Toggle full screen mode.
		*/
		void ToggleFullscreen();

		/*
			Get the monitor refresh rate.
		*/
		const float GetMonitorRefreshRate();

		/*
			Check if a window resize has been requested.
		*/
		void CheckForWindowResizeRequest();

		/*
			Render the current scene.
		*/
		void Render();
		
	private:

		/*
			Draw Render objects to the screen. Render objects are
			associated with a texture.
		*/
		void draw(RenderObject& object);

		/*
			Draw Debug objects to the screen. Debug objects are
			simple shapes drawn with solid colors.
		*/
		void draw(DebugObject& object);

		/*
			Draw Line objects to the screen. Line objects are
			simple lines drawn with solid colors.
		*/
		void draw(LineObject& line);

		/*
			Display the rendered objects to the screen.
		*/
		void display();

		/*
			Set the viewport dimensions based on the current window size
		*/
		void setViewport();

		/*
			Clear the screen before rendering.
		*/
		void clearScreen();

		/*
			Free renderer resources.
		*/
		void free();

		void* m_ptrWindow;
		void* m_ptrRenderer;

		bool m_rendererCreated;

		DebugColor m_lastDebugColor;

		ECS& m_refECS;
		AssetManager& m_refAssetManager;
	};
}