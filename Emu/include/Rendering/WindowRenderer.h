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

		void Initialize();

		void SetViewport();
		void ToggleFullscreen();

		const float GetMonitorRefreshRate();

		void Render();
		void Draw(Transform& refTransform, Animations* ptrAnimations, const int pixelsPerUnit, const Vector2D<float> offset);
		void Draw(ChainCollider& transform, const int pixelsPerUnit, const Vector2D<float> offset);
		void Display();

		void ClearScreen();

		void free();

	public:
		void* m_ptrWindow;
		void* m_ptrRenderer;

		bool m_rendererCreated;

		ECS& m_refECS;
		AssetManager& m_refAssetManager;

		std::vector<Entity> m_sortedEntitiesToRender;
	};
}