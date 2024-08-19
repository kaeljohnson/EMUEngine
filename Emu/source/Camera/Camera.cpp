#pragma once

#include "../../include/Camera/Camera.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Rendering/Screen.h"

namespace Engine
{
	Camera::Camera() : m_pixelsPerUnit(0), 
		m_offset(0, 0), m_size(0, 0), m_clampingOn(true) {}

	void Camera::SetCameraPosition(const Vector2D<float> offset)
	{
		m_offset = offset;
	}

	void Camera::Frame(const Vector2D<int> mapBounds, const int pixelsPerUnit)
	{
		m_mapBounds = mapBounds;

		m_size = Vector2D<float>(Screen::SCREEN_SIZE.X / (pixelsPerUnit * Screen::SCALE.X), Screen::SCREEN_SIZE.Y / (pixelsPerUnit * Screen::SCALE.Y));
	}

	void Camera::SetClampingOn(const bool clampingOn)
	{
		m_clampingOn = clampingOn;
	}

	void Camera::Clamp()
	{
		if (m_offset.X < 0) { m_offset.X = 0; }
		if (m_offset.X + m_size.X > m_mapBounds.X) { m_offset.X = m_mapBounds.X - m_size.X; }

		if (m_offset.Y < 0) { m_offset.Y = 0; }
		if (m_offset.Y + m_size.Y > m_mapBounds.Y) { m_offset.Y = m_mapBounds.Y - m_size.Y; }
	}

	void Camera::Update(double interpolation) {}
}