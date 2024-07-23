#pragma once

#include "../../include/Camera/Camera.h"
#include "../../include/Camera/ScrollingCamera.h"

namespace Engine
{
	ScrollingCamera::ScrollingCamera() : m_horizontalScrollSpeed(0), m_verticalScrollSpeed(0), Camera() {}

	void ScrollingCamera::Update(const double interpolation)
	{
		m_offsetX += m_horizontalScrollSpeed;
		m_offsetY += m_verticalScrollSpeed;
		if (m_clampingOn) Clamp();
	}

	void ScrollingCamera::SetScrollingSpeeds(const float xScrollingSpeed, const float yScrollingSpeed)
	{
		m_horizontalScrollSpeed = xScrollingSpeed;
		m_verticalScrollSpeed = yScrollingSpeed;
	}
}
