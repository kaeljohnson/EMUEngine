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

		void Render();
		void Draw(Transform& refTransform, Animations* ptrAnimations, const int pixelsPerUnit, const Vector2D<float> offset);
		void Draw(ChainCollider& transform, const int pixelsPerUnit, const Vector2D<float> offset);
		void Draw(RenderObject& object);
		void Draw(DebugObject& object);
		void Draw(Vector2D<int>& line);

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