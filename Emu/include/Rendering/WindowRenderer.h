#pragma once

#include <memory>

#include "../MathUtil.h"

#include "Screen.h"

#include "../Scenes/Scene.h"
#include "../Camera/Camera.h"

#include "../Transform.h"

namespace Engine
{
	class ISDL;

	class WindowRenderer
	{
	public: 
		WindowRenderer();
		~WindowRenderer();

		void RenderScene(std::shared_ptr<Scene> currentScene, const double interpolation);
		void Draw(Transform* sceneObject, const int pixelsPerUnit, const double interpolation, const Vector2D<float> offset);
		void Display();

		void SetViewport();
		void SetCamera(Camera* refCurrentCamera);
		void ResizeWindow(const int newWindowWidth, const int newWindowHeight);
		void ToggleFullscreen();

		void ClearScreen();

		void free();

	public:
		void* m_ptrWindow;
		void* m_ptrRenderer;

		Camera* ptrCurrentCamera;

		bool m_rendererCreated;
	};
}