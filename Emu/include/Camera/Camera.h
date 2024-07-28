#pragma once

#include "../Core.h"

#include "../MathUtil.h"

namespace Engine
{
	class Camera
	{
	public:
		EMU_API Camera();
		
		EMU_API void SetCameraPosition(const Vector2D offset);
		EMU_API void SetClampingOn(const bool clampingOn);

		EMU_API void Clamp();

		virtual ~Camera() = default;

	public:
		Vector2D m_offset;

	public:
		virtual void Update(const float interpolation);

		void Frame(const int pixelsPerMeter, const Vector2D mapBounds,
			const Vector2D screenSize, const Vector2D scale);

		Camera(const Camera& camera) = delete;
		Camera& operator=(const Camera&) = delete;
		Camera(Camera&&) = delete;
		Camera& operator=(Camera&&) = delete;
	protected:
		Vector2D m_sizeInMeters;

		// int m_mapBoundRight;
		// int m_mapBoundBottom;
		Vector2D m_mapBounds;
		
		bool m_clampingOn;

		// Rendering viewport scale. The viewport is not necessarily the same as the window size.
		Vector2D refScale;

		Vector2D m_screenSize;

		int m_pixelsPerMeter;
	};
}