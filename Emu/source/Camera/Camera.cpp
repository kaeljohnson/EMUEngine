#pragma once

#include "../../include/Camera/Camera.h"
#include "../../include/Logging/Logger.h"

// TEMP
#include "../../include/RendererManager.h"

namespace Engine
{
	Camera::Camera() : refScale(RendererManager::GetInstance()->GetScale()), 
		m_offset(0, 0), m_sizeInMeters(0, 0), m_clampingOn(true) {}

	void Camera::SetCameraPosition(const float x, const float y)
	{
		m_offset.X = x;
		m_offset.Y = y;
	}

	void Camera::Frame(const int pixelsPerMeter, const int levelWidthInMeters, const int levelHeightInMeters,
		const int screenWidth, const int screenHeight, const Vector2D scale)
	{
		m_mapBoundRight = levelWidthInMeters;
		m_mapBoundBottom = levelHeightInMeters;

		// SCALEX = scaleX;
		// SCALEY = scaleY;
		refScale = scale;

		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		m_pixelsPerMeter = pixelsPerMeter;

		m_sizeInMeters = Vector2D((float)screenWidth / (pixelsPerMeter * scale.X), (float)screenHeight / (pixelsPerMeter * scale.Y));
	}

	void Camera::SetClampingOn(const bool clampingOn)
	{
		m_clampingOn = clampingOn;
	}

	void Camera::Clamp()
	{
		if (m_offset.X < 0) { m_offset.X = 0; }
		if (m_offset.X + m_sizeInMeters.X > m_mapBoundRight) { m_offset.X = m_mapBoundRight - m_sizeInMeters.X; }

		if (m_offset.Y < 0) { m_offset.Y = 0; }
		if (m_offset.Y + m_sizeInMeters.Y > m_mapBoundBottom) { m_offset.Y = m_mapBoundBottom - m_sizeInMeters.Y; }
	}

	void Camera::Update(float interpolation) {}
}