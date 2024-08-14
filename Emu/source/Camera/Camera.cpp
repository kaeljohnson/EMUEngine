#pragma once

#include "../../include/Camera/Camera.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	Camera::Camera() : m_pixelsPerUnit(0), refScale(Vector2D(0.0f, 0.0f)), 
		m_offset(0, 0), m_size(0, 0), m_cameraMaxBound(0.0f, 0.0f) {}

	void Camera::SetCameraPosition(const Vector2D<float> offset)
	{
		m_offset = offset;
	}

	void Camera::Frame(const int pixelsPerUnit, const Vector2D<int> mapBounds,
		const Vector2D<int> screenSize, const Vector2D<float> scale)
	{
		m_mapBounds = mapBounds;

		refScale = scale;

		m_screenSize = screenSize;

		m_pixelsPerUnit = pixelsPerUnit;

		m_size = Vector2D<float>(screenSize.X / (pixelsPerUnit * scale.X), screenSize.Y / (pixelsPerUnit * scale.Y));

		m_cameraMaxBound = m_mapBounds - m_size;
	}

	void Camera::Clamp()
	{
		clamp(m_offset, Vector2D(0.0f, 0.0f), m_cameraMaxBound);

		/*if (m_offset.X < 0) { m_offset.X = 0; }
		if (m_offset.X + m_size.X > m_mapBounds.X) { m_offset.X = m_mapBounds.X - m_size.X; }

		if (m_offset.Y < 0) { m_offset.Y = 0; }
		if (m_offset.Y + m_size.Y > m_mapBounds.Y) { m_offset.Y = m_mapBounds.Y - m_size.Y; }*/
	}

	void Camera::Update(double interpolation) {}
}