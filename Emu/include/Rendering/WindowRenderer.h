#pragma once

#include "../SDLWrapper/SDLWrapper.h"

#include <memory>

#include "../MathUtil.h"

#include "../Scenes/SceneObject.h"
#include "../Scenes/Scene.h"

namespace Engine
{
	class WindowRenderer
	{
	public: 
		WindowRenderer();
		~WindowRenderer();

		void RenderScene(std::shared_ptr<Scene> currentScene, const double interpolation, const Math::Vector2D<float> offset);
		void Draw(SceneObject* sceneObject, const int pixelsPerUnit, const double interpolation, const Math::Vector2D<float> offset);
		void Display();

		void SetViewport();
		void ResizeWindow(const int newWindowWidth, const int newWindowHeight);
		void ToggleFullscreen();

		void ClearScreen();

		void free();

	public:
		// May need to decouple if there is ever a need for 
		// the application to have multiple windows.
		SDLWindow* m_ptrWindow;
		SDLRenderer* m_ptrRenderer;

		Math::Vector2D<int> m_screenSize;
		Math::Vector2D<int> m_virtualSize;
		Math::Vector2D<float> m_scale;
		float SCALE;

		Math::Vector2D<int> m_viewportSize;
		Math::Vector2D<int> m_viewportPosition;

		bool m_rendererCreated;
	};
}