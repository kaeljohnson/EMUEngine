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
		EMU_API void SetScrollingSpeeds(const Vector2D scrollSpeed);

		~ScrollingCamera() = default;

	public:
		void Update(const float interpolation) override;

	private:
		Vector2D m_scrollSpeed;
	};
}