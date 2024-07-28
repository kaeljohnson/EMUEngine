#pragma once

#include "../../include/Camera/Camera.h"
#include "../../include/Logging/Logger.h"

// TEMP
#include "../../include/RendererManager.h"

namespace Engine
{
	Camera::Camera() : m_pixelsPerMeter(0), refScale(RendererManager::GetInstance()->GetScale()), 
		m_offset(0, 0), m_sizeInMeters(0, 0), m_clampingOn(true) {}

	void Camera::SetCameraPosition(const Vector2D<float> offset)
	{
		m_offset = offset;
	}

	void Camera::Frame(const int pixelsPerMeter, const Vector2D<int> mapBounds,
		const Vector2D<int> screenSize, const Vector2D<float> scale)
	{
		m_mapBounds = mapBounds;

		refScale = scale;

		m_screenSize = screenSize;

		m_pixelsPerMeter = pixelsPerMeter;

		m_sizeInMeters = Vector2D((float)screenSize.X / (pixelsPerMeter * scale.X), (float)screenSize.Y / (pixelsPerMeter * scale.Y));
	}

	void Camera::SetClampingOn(const bool clampingOn)
	{
		m_clampingOn = clampingOn;
	}

	void Camera::Clamp()
	{
		if (m_offset.X < 0) { m_offset.X = 0; }
		if (m_offset.X + m_sizeInMeters.X > m_mapBounds.X) { m_offset.X = m_mapBounds.X - m_sizeInMeters.X; }

		if (m_offset.Y < 0) { m_offset.Y = 0; }
		if (m_offset.Y + m_sizeInMeters.Y > m_mapBounds.Y) { m_offset.Y = m_mapBounds.Y - m_sizeInMeters.Y; }
	}

	void Camera::Update(float interpolation) {}
}