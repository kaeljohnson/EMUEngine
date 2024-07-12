#pragma once

#include "../../include/Camera/Camera.h"
#include "../../include/Logging/Logger.h"

// TEMP
#include "../../include/RendererManager.h"

namespace Engine
{
	Camera::Camera() : SCALEX(RendererManager::GetInstance()->GetScaleX()), SCALEY(RendererManager::GetInstance()->GetScaleY()), 
		m_offsetX(0), m_offsetY(0), m_widthInMeters(0), m_heightInMeters(0), m_clampingOn(true) {}

	void Camera::SetCameraPosition(const float x, const float y)
	{
		m_offsetX = x;
		m_offsetY = y;
	}

	void Camera::Frame(const int pixelsPerMeter, const int levelWidthInMeters, const int levelHeightInMeters,
		const int screenWidth, const int screenHeight, const float scaleX, const float scaleY)
	{
		m_mapBoundRight = levelWidthInMeters;
		m_mapBoundBottom = levelHeightInMeters;

		SCALEX = scaleX;
		SCALEY = scaleY;

		m_widthInMeters = (float)screenWidth / (pixelsPerMeter * SCALEX);
		m_heightInMeters = (float)screenHeight / (pixelsPerMeter * SCALEY);
	}

	void Camera::SetClampingOn(const bool clampingOn)
	{
		m_clampingOn = clampingOn;
	}

	void Camera::Clamp()
	{
		if (m_offsetX < 0) { m_offsetX = 0; }
		if (m_offsetX + m_widthInMeters > m_mapBoundRight) { m_offsetX = m_mapBoundRight - m_widthInMeters; }

		if (m_offsetY < 0) { m_offsetY = 0; }
		if (m_offsetY + m_heightInMeters > m_mapBoundBottom) { m_offsetY = m_mapBoundBottom - m_heightInMeters; }
	}

	void Camera::Update(double interpolation) {}
}