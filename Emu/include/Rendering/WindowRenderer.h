#pragma once

#include "../ECS/ECS.h"
#include "../Includes.h"
#include "../MathUtil.h"
#include "../Components.h"
#include "../AssetManager.h"
#include "Screen.h"

namespace Engine
{
	class WindowRenderer
	{
	public:
		WindowRenderer(ECS& refECS, AssetManager& refAssetManager);
		~WindowRenderer();

		void SetViewport();
		void ToggleFullscreen();

		const float GetMonitorRefreshRate();
		void CheckForWindowResizeRequest();

		void Render();
		void Draw(RenderObject& object);
		void Draw(DebugObject& object);
		void Draw(LineObject& line);
		void Display();

		void ClearScreen();

		void free();

	public:
		void* m_ptrWindow;
		void* m_ptrRenderer;

		bool m_rendererCreated;

		ECS& m_refECS;
		AssetManager& m_refAssetManager;
	};
}