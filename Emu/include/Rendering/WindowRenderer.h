#pragma once

#include <memory>

#include "../MathUtil.h"

#include "Screen.h"

#include "../Transform.h"

namespace Engine
{
	class ISDL;

	class WindowRenderer
	{
	public: 
		WindowRenderer();
		~WindowRenderer();

		void RenderScene(const size_t currentEntityID);
		void Draw(Transform& transform, const int pixelsPerUnit, const Vector2D<float> offset);
		void Display();

		void SetViewport();
		void ResizeWindow(const int newWindowWidth, const int newWindowHeight);
		void ToggleFullscreen();

		void ClearScreen();

		void free();

	public:
		void* m_ptrWindow;
		void* m_ptrRenderer;

		bool m_rendererCreated;
	};
}