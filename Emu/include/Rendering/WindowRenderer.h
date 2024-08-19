#pragma once

#include <memory>

#include "../MathUtil.h"

#include "Screen.h"

#include "../Scenes/SceneObject.h"
#include "../Scenes/Scene.h"

namespace Engine
{
	class ISDL;

	class WindowRenderer
	{
	public: 
		WindowRenderer();
		~WindowRenderer();

		void RenderScene(std::shared_ptr<Scene> currentScene, const double interpolation, const Vector2D<float> offset);
		void Draw(SceneObject* sceneObject, const int pixelsPerUnit, const double interpolation, const Vector2D<float> offset);
		void Display();

		void SetViewport();
		void ResizeWindow(const int newWindowWidth, const int newWindowHeight);
		void ToggleFullscreen();

		const int GetFullscreenWidth() const;
		const int GetFullscreenHeight() const;

		const Vector2D<float> GetScale() const;

		void ClearScreen();

		void free();

	public:
		void* m_ptrWindow;
		void* m_ptrRenderer;

		bool m_rendererCreated;
	};
}