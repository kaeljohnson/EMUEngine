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
		Camera(float x, float y, float w, float h, const int levelWidth, const int levelHeight);

		EMU_API void SetCameraTarget(SceneObject* ptrTarget);
		
		void SetPixelsPerMeter(const int pixelsPerMeter);
		void SetLevelWidthInMeters(const int levelWidthInMeters);
		void SetLevelHeightInMeters(const int levelHeightInMeters);

		void Update(const double interpolation);
		void Clamp();

		inline const float GetXValue() const { return m_x; };
		inline const float GetYValue() const { return m_y; };
		inline const float GetWidth() const { return m_width; };
		inline const float GetHeight() const { return m_height; };

		Camera(const Camera& camera) = delete;
		Camera& operator=(const Camera&) = delete;
		Camera(Camera&&) = delete;
		Camera& operator=(Camera&&) = delete;

	private:

		// Default camera behavior is to follow a target.
		// If the camera is not following a target, it will be static.
		// The client should be able to set other behaviors like: continuous scrolling, etc.
		SceneObject* ptrCameraTarget;

		int m_levelWidthInMeters;
		int m_levelHeightInMeters;
		
		float m_width;
		float m_height;

		int m_levelWidth;
		int m_levelHeight;

		int m_pixelsPerMeter;
	public:
		float m_offsetX;
		float m_offsetY;

		float m_x;
		float m_y;
	};
}