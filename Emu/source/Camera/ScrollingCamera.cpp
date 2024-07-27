#pragma once

#include "../../include/Camera/Camera.h"
#include "../../include/Camera/ScrollingCamera.h"

namespace Engine
{
	ScrollingCamera::ScrollingCamera() : m_scrollSpeed(0, 0), Camera() {}

	void ScrollingCamera::Update(const float interpolation)
	{
		m_offset += m_scrollSpeed;
		if (m_clampingOn) Clamp();
	}

	void ScrollingCamera::SetScrollingSpeeds(const Vector2D scrollSpeed)
	{
		m_scrollSpeed = scrollSpeed;
	}
}
