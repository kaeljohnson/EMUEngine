#pragma once

#include "../Core.h"

namespace Engine
{
	class Camera
	{
	public:
		EMU_API Camera();
		
		EMU_API void SetCameraPosition(const float x, const float y);
		EMU_API void SetClampingOn(const bool clampingOn);

		virtual ~Camera() = default;

	public:
		float m_offsetX;
		float m_offsetY;

	public:
		virtual void Update(const double interpolation);

		void Frame(const int pixelsPerMeter, const float levelWidthInMeters, const float levelHeightInMeters,
			const float scaleX, const float scaleY, const float screenWidth, const float screenHeight);
		
		void Clamp();

		Camera(const Camera& camera) = delete;
		Camera& operator=(const Camera&) = delete;
		Camera(Camera&&) = delete;
		Camera& operator=(Camera&&) = delete;
	protected:
		float m_widthInMeters;
		float m_heightInMeters;

		float m_mapBoundRight;
		float m_mapBoundBottom;
		
		bool m_clampingOn;

		// Rendering viewport scale. The viewport is not necessarily the same as the window size.
		float SCALEX;
		float SCALEY;
	};
}