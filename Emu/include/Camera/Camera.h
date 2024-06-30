#pragma once

#include <memory>

#include "../Core.h"

#include "../Scenes/SceneObject.h"

namespace Engine
{
	class Camera
	{
	public:
		EMU_API Camera();
		EMU_API Camera(float x, float y, int w, int h, const int levelWidth, const int levelHeight, const float SCALEX, const float SCALEY);
		EMU_API void SetCameraTarget(SceneObject* ptrTarget);
		EMU_API void SetClampingOn(const bool clampingOn);

	public:
		float m_offsetX;
		float m_offsetY;

		float m_x;
		float m_y;
	public:
		void Frame(const int pixelsPerMeter, const float levelWidthInMeters, const float levelHeightInMeters,
			const float scaleX, const float scaleY, const float screenWidth, const float screenHeight);

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

		// Default camera behavior is to be static.
		// If the camera is not following a target, it will be static, or scrolling.
		// The client should be able to set other behaviors like: continuous scrolling, etc.
		SceneObject* ptrCameraTarget;

		
		int m_width;
		int m_height;

		float m_widthInMeters;
		float m_heightInMeters;

		int currentScenesPixelsPerMeter;
		float currentScenesMapWidthPerMeter;
		float currentScenesMapHeightPerMeter;

		// Rendering viewport scale. The viewport is not necessarily the same as the window size.
		float refSCALEX;
		float refSCALEY;

		bool m_clampingOn;
	};
}