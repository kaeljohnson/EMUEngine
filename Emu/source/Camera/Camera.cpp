#pragma once

#include "../../include/Camera/Camera.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	Camera::Camera() : m_pixelsPerUnit(0), refScale(Math::Vector2D(0.0f, 0.0f)), 
		m_offset(0, 0), m_size(0, 0), m_cameraMaxBound(0.0f, 0.0f) {}

	void Camera::SetCameraPosition(const Math::Vector2D<float> offset)
	{
		m_offset = offset;
	}

	void Camera::Frame(const int pixelsPerUnit, const Math::Vector2D<int> mapBounds,
		const Math::Vector2D<int> screenSize, const Math::Vector2D<float> scale)
	{
		refScale = scale;

		m_screenSize = screenSize;

		m_pixelsPerUnit = pixelsPerUnit;

		m_size = Math::Vector2D<float>(screenSize.X / (pixelsPerUnit * scale.X), screenSize.Y / (pixelsPerUnit * scale.Y));

		m_cameraMaxBound = mapBounds - m_size;
	}

	void Camera::Clamp()
	{
		Math::Clamp(m_offset, Math::Vector2D(0.0f, 0.0f), m_cameraMaxBound);
	}

	void Camera::Update(double interpolation) {}
}