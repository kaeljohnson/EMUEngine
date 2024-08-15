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

	protected:
		Math::Vector2D<float> m_size;
		Math::Vector2D<float> m_cameraMaxBound;
		Math::Vector2D<float> m_scale; // Rendering viewport scale.
		Math::Vector2D<int> m_screenSize;
		Math::Vector2D<float> m_offset;

	public:
		virtual void Update(const double interpolation);

		void Frame(const int pixelsPerUnit, const Math::Vector2D<int> mapBounds,
			const Math::Vector2D<int> screenSize, const Math::Vector2D<float> scale);

		inline const Math::Vector2D<float>& GetOffset() const { return m_offset; }

		Camera(const Camera& camera) = delete;
		Camera& operator=(const Camera&) = delete;
		Camera(Camera&&) = delete;
		Camera& operator=(Camera&&) = delete;
	};
}