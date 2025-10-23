#pragma once

#include "../Core.h"
#include "../MathUtil.h"

namespace Engine
{
	// Interface for client to set window size and fullscreen along with other misc screen properties
	// Should this class be un-staticified and given a pointer to the window?
	struct Screen
	{
		inline static const Vector2D<int> GetScreenSize() { return DISPLAY_RESOLUTION; }
		inline static const Vector2D<int> GetVirtualSize() { return VIRTUAL_SIZE; }
		inline static const Vector2D<float> GetScale() { return SCALE; }
		inline static const float GetScaleConstant() { return SCALE_CONSTANT; }
		
		inline static const Vector2D<int> GetViewportSize() { return VIEWPORT_SIZE; }
		inline static const Vector2D<int> GetViewportPosition() { return VIEWPORT_POSITION; }

		inline static void SetWindowSize(const Vector2D<int>& size) { VIEWPORT_SIZE = size; WINDOW_RESIZE_REQUEST = true; }
		inline static void SetFullscreen() { TOGGLE_FULLSCREEN_REQUEST = true; }

	private:

		static bool WINDOW_RESIZE_REQUEST;
		static bool TOGGLE_FULLSCREEN_REQUEST;

		static Vector2D<int> DISPLAY_RESOLUTION;
		static Vector2D<int> VIRTUAL_SIZE;
		static Vector2D<float> SCALE;
		static float SCALE_CONSTANT;

		static Vector2D<int> VIEWPORT_SIZE;
		static Vector2D<int> VIEWPORT_POSITION;

		friend class IRenderer;
		friend class CameraInterface;
		friend class CameraSystem;
	};
}