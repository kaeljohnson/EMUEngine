#pragma once

#include "../Core.h"
#include "../MathUtil.h"

namespace Engine
{
	struct Screen
	{
		EMU_API inline static const Vector2D<int> GetScreenSize() { return SCREEN_SIZE; }
		EMU_API inline static const Vector2D<int> GetVirtualSize() { return VIRTUAL_SIZE; }
		EMU_API inline static const Vector2D<float> GetScale() { return SCALE; }
		EMU_API inline static const float GetScaleConstant() { return SCALE_CONSTANT; }
		
		EMU_API inline static const Vector2D<int> GetViewportSize() { return VIEWPORT_SIZE; }
		EMU_API inline static const Vector2D<int> GetViewportPosition() { return VIEWPORT_POSITION; }

		EMU_API inline static void SetWindowSize(const Vector2D<int>& size) { SCREEN_SIZE = size; WINDOW_RESIZE_REQUEST = true; }
		EMU_API inline static void SetFullscreen() { TOGGLE_FULLSCREEN_REQUEST = true; }

	private:

		static bool WINDOW_RESIZE_REQUEST;
		static bool TOGGLE_FULLSCREEN_REQUEST;

		static Vector2D<int> SCREEN_SIZE;
		static Vector2D<int> VIRTUAL_SIZE;
		static Vector2D<float> SCALE;
		static float SCALE_CONSTANT;

		static Vector2D<int> VIEWPORT_SIZE;
		static Vector2D<int> VIEWPORT_POSITION;

		friend class WindowRenderer;
		friend class Camera;
	};
}