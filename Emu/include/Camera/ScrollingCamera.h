#pragma once

#include "../Core.h"

#include "Camera.h"

namespace Engine
{
	class ScrollingCamera : public Camera
	{
	public:
		EMU_API ScrollingCamera();
		EMU_API void SetScrollingSpeeds(const float xScrollingSpeed, const float yScrollingSpeed);

		~ScrollingCamera() = default;

	public:
		void Update(const double interpolation) override;

	private:
		float m_horizontalScrollSpeed;
		float m_verticalScrollSpeed;
	};
}