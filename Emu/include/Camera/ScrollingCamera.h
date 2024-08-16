#pragma once

#include "../Core.h"
#include "../MathUtil.h"
#include "Camera.h"

namespace Engine
{
	class ScrollingCamera : public Camera
	{
	public:
		EMU_API ScrollingCamera();
		EMU_API void SetScrollingSpeeds(const Math::Vector2D<float> scrollSpeed);

		~ScrollingCamera() = default;

	public:
		void Update(const double interpolation) override;

	private:
		Math::Vector2D<float> m_scrollSpeed;
	};
}