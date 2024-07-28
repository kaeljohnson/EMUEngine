#pragma once

#include "../Core.h"

#include "../MathUtil.h"

namespace Engine
{
	class Camera
	{
	public:
		EMU_API Camera();
		
		EMU_API void SetCameraPosition(const Vector2D<float> offset);
		EMU_API void SetClampingOn(const bool clampingOn);

		EMU_API void Clamp();

		virtual ~Camera() = default;

	public:
		Vector2D<float> m_offset;

	public:
		virtual void Update(const double interpolation);

		void Frame(const int pixelsPerMeter, const Vector2D<int> mapBounds,
			const Vector2D<int> screenSize, const Vector2D<float> scale);

		Camera(const Camera& camera) = delete;
		Camera& operator=(const Camera&) = delete;
		Camera(Camera&&) = delete;
		Camera& operator=(Camera&&) = delete;
	protected:
		Vector2D<float> m_sizeInMeters;

		Vector2D<int> m_mapBounds;

		// Rendering viewport scale. The viewport is not necessarily the same as the window size.
		Vector2D<float> refScale;

		Vector2D<int> m_screenSize;

		int m_pixelsPerMeter;
		bool m_clampingOn;
	};
}