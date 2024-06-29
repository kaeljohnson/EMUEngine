#pragma once

#include <memory>

#include "../Core.h"

#include "../Scenes/SceneObject.h"

namespace Engine
{
	class Camera
	{
	public:
		Camera();
		Camera(float x, float y, int w, int h, const int levelWidth, const int levelHeight, const float SCALE);

		EMU_API void SetCameraTarget(SceneObject* ptrTarget);
		EMU_API void SetClampingOn(const bool clampingOn);
		
		void SetPixelsPerMeter(const int pixelsPerMeter);
		void SetLevelWidthInMeters(const int levelWidthInMeters);
		void SetLevelHeightInMeters(const int levelHeightInMeters);

		void Update(const double interpolation);
		void Clamp();

		inline const float GetXValue() const { return m_x; };
		inline const float GetYValue() const { return m_y; };
		inline const int GetWidth() const { return m_width; };
		inline const int GetHeight() const { return m_height; };

		Camera(const Camera& camera) = delete;
		Camera& operator=(const Camera&) = delete;
		Camera(Camera&&) = delete;
		Camera& operator=(Camera&&) = delete;

	private:

		// Default camera behavior is to follow a target.
		// If the camera is not following a target, it will be static.
		// The client should be able to set other behaviors like: continuous scrolling, etc.
		SceneObject* ptrCameraTarget;
		
		int m_width;
		int m_height;

		int m_levelWidth;
		int m_levelHeight;

		int m_pixelsPerMeter;

		// Rendering scale. The viewport is not necessarily the same as the window size.
		float refSCALE;

		bool m_clampingOn;
	public:
		float m_offsetX;
		float m_offsetY;

		float m_x;
		float m_y;
	};
}