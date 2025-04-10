#pragma once

#include "../ECS/ECS.h"
#include "../Includes.h"
#include "../MathUtil.h"
#include "../Components.h"
#include "Screen.h"

namespace Engine
{
	class WindowRenderer
	{
	public:
		WindowRenderer(ECS& refECS);
		~WindowRenderer();

		void Initialize();
		void Activate(Entity entity);
		void Deactivate(Entity entity);

		void SetViewport();
		void ResizeWindow(const int newWindowWidth, const int newWindowHeight);
		void ToggleFullscreen();

		const float GetMonitorRefreshRate();

		void Render();
		void Draw(Transform& transform, const int pixelsPerUnit, const Vector2D<float> offset);
		void Draw(ChainCollider& transform, const int pixelsPerUnit, const Vector2D<float> offset);
		void Display();

		void ClearScreen();

		void free();

	public:
		void* m_ptrWindow;
		void* m_ptrRenderer;

		bool m_rendererCreated;

		ECS& m_refECS;

		std::vector<Entity> m_sortedEntitiesToRender;
	};
}