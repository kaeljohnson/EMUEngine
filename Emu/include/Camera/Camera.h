#pragma once

#include "../Core.h"

#include "../MathUtil.h"

namespace Engine
{
	class Camera
	{
	public:
		EMU_API Camera();
		
		EMU_API void SetCameraPosition(const Math::Vector2D<float> offset);

		EMU_API void Clamp();

		virtual ~Camera() = default;

	public:
		Math::Vector2D<float> m_offset;

	public:
		virtual void Update(const double interpolation);

		void Frame(const int pixelsPerUnit, const Math::Vector2D<int> mapBounds,
			const Math::Vector2D<int> screenSize, const Math::Vector2D<float> scale);

		Camera(const Camera& camera) = delete;
		Camera& operator=(const Camera&) = delete;
		Camera(Camera&&) = delete;
		Camera& operator=(Camera&&) = delete;
	protected:
		Math::Vector2D<float> m_size;

		Math::Vector2D<float> m_cameraMaxBound;

		// Rendering viewport scale. The viewport is not necessarily the same as the window size.
		Math::Vector2D<float> refScale;

		Math::Vector2D<int> m_screenSize;

		int m_pixelsPerUnit;
	};
}