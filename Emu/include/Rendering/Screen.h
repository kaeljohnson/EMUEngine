#pragma once

#include "../Core.h"
#include "../MathUtil.h"

namespace Engine
{

	// Interface for client to set window size and fullscreen along with other misc screen properties
	// Should this class be un-staticified and given a pointer to the window?
	struct Screen
	{
		EMU_API inline static const Vector2D<int> GetScreenSize() { return DISPLAY_RESOLUTION; }
		EMU_API inline static const Vector2D<int> GetVirtualSize() { return VIRTUAL_SIZE; }
		EMU_API inline static const Vector2D<float> GetScale() { return SCALE; }
		EMU_API inline static const float GetScaleConstant() { return SCALE_CONSTANT; }
		
		EMU_API inline static const Vector2D<int> GetViewportSize() { return VIEWPORT_SIZE; }
		EMU_API inline static const Vector2D<int> GetViewportPosition() { return VIEWPORT_POSITION; }

		EMU_API inline static void SetWindowSize(const Vector2D<int>& size) { VIEWPORT_SIZE = size; WINDOW_RESIZE_REQUEST = true; }
		EMU_API inline static void SetFullscreen() { TOGGLE_FULLSCREEN_REQUEST = true; }

	private:

		static bool WINDOW_RESIZE_REQUEST;
		static bool TOGGLE_FULLSCREEN_REQUEST;

		static Vector2D<int> DISPLAY_RESOLUTION;
		static Vector2D<int> VIRTUAL_SIZE;
		static Vector2D<float> SCALE;
		static float SCALE_CONSTANT;

		static Vector2D<int> VIEWPORT_SIZE;
		static Vector2D<int> VIEWPORT_POSITION;

		friend class WindowRenderer;
		friend class CameraInterface;
		friend class CameraSystem;
	};
}