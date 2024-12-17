#pragma once

#include "../ECS/Entity.h"
#include "../Includes.h"
#include "../MathUtil.h"
#include "../Components.h"
#include "Screen.h"

namespace Engine
{
	//class ISDL;

	class WindowRenderer
	{
	public:
		// Client interface for now
		EMU_API void SetViewport();
		EMU_API void ResizeWindow(const int newWindowWidth, const int newWindowHeight);
		EMU_API void ToggleFullscreen();

	public: 
		WindowRenderer();
		~WindowRenderer();

		void Render(Entity* currentEntity);
		void Draw(Transform& transform, const int pixelsPerUnit, const Vector2D<float> offset);
		void Display();

		void ClearScreen();

		void free();

	public:
		void* m_ptrWindow;
		void* m_ptrRenderer;

		bool m_rendererCreated;
	};
}