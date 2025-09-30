#pragma once

#include "../ECS/ECS.h"
#include "../Includes.h"
#include "../Components.h"
#include "../AssetManager.h"
#include "Screen.h"

namespace Engine
{
	class IRenderer
	{
	public:
		IRenderer(ECS& refECS, AssetManager& refAssetManager);
		~IRenderer();

		void SetViewport();
		void ToggleFullscreen();

		const float GetMonitorRefreshRate();
		void CheckForWindowResizeRequest();

		void Render();
		
	private:
		void draw(RenderObject& object);
		void draw(DebugObject& object);
		void draw(LineObject& line);
		void display();

		void clearScreen();

		void free();

		void* m_ptrWindow;
		void* m_ptrRenderer;

		bool m_rendererCreated;

		ECS& m_refECS;
		AssetManager& m_refAssetManager;
	};
}