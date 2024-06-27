#pragma once

// TEMP
#include "../../include/RendererManager.h"

#include "../../include/Camera/Camera.h"

#include "../../include/Logging/Logger.h"


namespace Engine
{
	Camera::Camera() : m_x(0), m_y(0), m_width(0), m_height(0), m_levelWidth(0), m_levelHeight(0), m_offsetX(0), m_offsetY(0), ptrCameraTarget(nullptr) {}

	Camera::Camera(float x, float y, const float w, const float h, const int levelWidth, const int levelHeight)
		: m_x(x), m_y(y), m_width(w), m_height(h), m_offsetX(0), m_offsetY(0), m_levelWidth(levelWidth), m_levelHeight(levelHeight), ptrCameraTarget(nullptr) {}

	void Camera::SetCameraTarget(SceneObject* ptrTarget)
	{
		ptrCameraTarget = ptrTarget;
	}

	void Camera::SetPixelsPerMeter(const int pixelsPerMeter)
	{
		m_pixelsPerMeter = pixelsPerMeter;
	}

	void Camera::SetLevelWidthInMeters(const int levelWidthInMeters) { m_levelWidth = levelWidthInMeters; };
	void Camera::SetLevelHeightInMeters(const int levelHeightInMeters) { m_levelHeight = levelHeightInMeters; };

	void Camera::Clamp()
	{
		if (m_offsetX < 0) { m_offsetX = 0; }
		if (m_offsetY < 0) { m_offsetY = 0; }
		if (m_offsetX + (m_width / (m_pixelsPerMeter * 2)) > m_levelWidth) { m_offsetX = (m_levelWidth) - (m_width / (m_pixelsPerMeter * 2)); }
		if (m_offsetY + (m_height / (m_pixelsPerMeter * 2)) > m_levelHeight) { m_offsetY = (m_levelHeight) - (m_height / (m_pixelsPerMeter * 2)); }
	}

	// Camera class method
	void Camera::Update(double interpolation)
	{
		// Calculate the interpolated position of the target
		float interpolatedX = (ptrCameraTarget->GetPhysicsBody()->GetTopLeftPrevX() * (1.0 - interpolation)) +
			(ptrCameraTarget->GetPhysicsBody()->GetTopLeftXInMeters() * interpolation);
		float interpolatedY = (ptrCameraTarget->GetPhysicsBody()->GetTopLeftPrevY() * (1.0 - interpolation)) +
			(ptrCameraTarget->GetPhysicsBody()->GetTopLeftYInMeters() * interpolation);

		// Update offsets based on the interpolated position
		m_offsetX = interpolatedX - (m_width / (m_pixelsPerMeter * 4));// -20.0f; // Assuming 40.0f is the desired offset; adjust as necessary
		m_offsetY = interpolatedY - (m_height / (m_pixelsPerMeter * 4));// -11.5f; // Assuming 22.0f is the desired offset; adjust as necessary

		Clamp();
	}
}