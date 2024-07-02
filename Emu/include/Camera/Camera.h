#pragma once

#include <memory>

#include "../Core.h"

#include "../Scenes/SceneObject.h"

namespace Engine
{
	// Need mix match types. I.E. Horizontal scroller, but vertical follow.
	enum CameraType { STATIC_CAMERA, FOLLOW_TARGET, SCROLLING };

	class Camera
	{
	public:
		EMU_API Camera();
		EMU_API Camera(CameraType cameraType);
		EMU_API void SetCameraTarget(SceneObject* ptrTarget);
		EMU_API void SetScrollingSpeeds(const float xScrollingSpeed, const float yScrollingSpeed);
		EMU_API void SetClampingOn(const bool clampingOn);

	public:
		const CameraType m_cameraType;

		float m_offsetX;
		float m_offsetY;

	public:
		void Frame(const int pixelsPerMeter, const float levelWidthInMeters, const float levelHeightInMeters,
			const float scaleX, const float scaleY, const float screenWidth, const float screenHeight);

		void Update(const double interpolation);
		void Clamp();

		Camera(const Camera& camera) = delete;
		Camera& operator=(const Camera&) = delete;
		Camera(Camera&&) = delete;
		Camera& operator=(Camera&&) = delete;
	private:

		// Default camera behavior is to be static.
		// If the camera is not following a target, it will be static, or scrolling.
		// The client should be able to set other behaviors like: continuous scrolling, etc.
		SceneObject* ptrCameraTarget;

		float m_horizontalScrollSpeed;
		float m_verticalScrollSpeed;
	
		float m_widthInMeters;
		float m_heightInMeters;

		float m_mapBoundRight;
		float m_mapBoundBottom;

		// Rendering viewport scale. The viewport is not necessarily the same as the window size.
		float SCALEX;
		float SCALEY;

		bool m_clampingOn;
	};
}