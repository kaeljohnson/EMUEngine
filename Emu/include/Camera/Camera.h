#pragma once

#include "../Core.h"

#include "../MathUtil.h"

namespace Engine
{
	class Camera
	{
	public:
		EMU_API Camera();
		
		EMU_API void SetPixelsPerUnit(const int pixelsPerUnit);
		EMU_API void SetCameraPosition(const Vector2D<float> offset);
		EMU_API void SetClampingOn(const bool clampingOn);
		EMU_API void SetSize();

		EMU_API inline const int GetPixelsPerUnit() const { return m_pixelsPerUnit; }

		EMU_API void Clamp();

		virtual ~Camera() = default;

	public:
		Vector2D<float> m_offset;

	public:
		virtual void Update(const double interpolation);

		void Frame(const Vector2D<int> mapBounds);

		Camera(const Camera& camera) = delete;
		Camera& operator=(const Camera&) = delete;
		Camera(Camera&&) = delete;
		Camera& operator=(Camera&&) = delete;
	protected:
		Vector2D<float> m_size;

		Vector2D<int> m_mapBounds;

		// Rendering viewport scale. The viewport is not necessarily the same as the window size.

		int m_pixelsPerUnit;
		bool m_clampingOn;
	};
}