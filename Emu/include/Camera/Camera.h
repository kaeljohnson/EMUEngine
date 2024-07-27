#pragma once

#include "../Core.h"

#include "../MathUtil.h"

namespace Engine
{
	class Camera
	{
	public:
		EMU_API Camera();
		
		EMU_API void SetCameraPosition(const float x, const float y);
		EMU_API void SetClampingOn(const bool clampingOn);

		EMU_API void Clamp();

		virtual ~Camera() = default;

	public:
		Vector2D m_offset;

	public:
		virtual void Update(const float interpolation);

		void Frame(const int pixelsPerMeter, const int levelWidthInMeters, const int levelHeightInMeters,
			const int screenWidth, const int screenHeight, const Vector2D scale);

		Camera(const Camera& camera) = delete;
		Camera& operator=(const Camera&) = delete;
		Camera(Camera&&) = delete;
		Camera& operator=(Camera&&) = delete;
	protected:
		// float m_widthInMeters;
		// float m_heightInMeters;
		Vector2D m_sizeInMeters;

		int m_mapBoundRight;
		int m_mapBoundBottom;
		
		bool m_clampingOn;

		// Rendering viewport scale. The viewport is not necessarily the same as the window size.
		// float SCALEX;
		// float SCALEY;

		Vector2D refScale;

		int m_screenWidth;
		int m_screenHeight;

		int m_pixelsPerMeter;
	};
}