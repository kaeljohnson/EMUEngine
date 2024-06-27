#pragma once

// TEMP
#include "../../include/RendererManager.h"

#include "../../include/Camera/Camera.h"

#include "../../include/Logging/Logger.h"


namespace Engine
{
	Camera::Camera() : m_x(0), m_y(0), m_width(0), m_height(0), m_levelWidth(0), m_levelHeight(0), m_offsetX(0), m_offsetY(0), m_pixelsPerMeter(0), m_clampingOn(true), ptrCameraTarget(nullptr) {}

	Camera::Camera(float x, float y, const int w, const int h, const int levelWidth, const int levelHeight)
		: m_x(x), m_y(y), m_width(w), m_height(h), m_offsetX(0), m_offsetY(0), m_levelWidth(levelWidth), m_levelHeight(levelHeight), m_pixelsPerMeter(0), ptrCameraTarget(nullptr) {}

	void Camera::SetCameraTarget(SceneObject* ptrTarget)
	{
		ptrCameraTarget = ptrTarget;
	}

	void Camera::SetClampingOn(const bool clampingOn)
	{
		m_clampingOn = clampingOn;
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
		if (m_offsetX + ((float)m_width / (m_pixelsPerMeter * 2)) > m_levelWidth) { m_offsetX = ((float)m_levelWidth) - ((float)m_width / (m_pixelsPerMeter * 2)); }
		if (m_offsetY + ((float)m_height / (m_pixelsPerMeter * 2)) > m_levelHeight) { m_offsetY = ((float)m_levelHeight) - ((float)m_height / (m_pixelsPerMeter * 2)); }
	}

	// Camera class method
	void Camera::Update(double interpolation)
	{
		if (ptrCameraTarget == nullptr)
		{
			ENGINE_CRITICAL_D("Camera target is nullptr. Camera is static!");
			return;
		}
		// Calculate the interpolated position of the target
		double interpolatedX = (ptrCameraTarget->GetPhysicsBody()->GetTopLeftPrevX() * (1.0f - interpolation)) +
			(ptrCameraTarget->GetPhysicsBody()->GetTopLeftXInMeters() * interpolation);
		double interpolatedY = (ptrCameraTarget->GetPhysicsBody()->GetTopLeftPrevY() * (1.0f - interpolation)) +
			(ptrCameraTarget->GetPhysicsBody()->GetTopLeftYInMeters() * interpolation);

		// Update offsets based on the interpolated position
		m_offsetX = (float)interpolatedX - (m_width / (m_pixelsPerMeter * 4));// -20.0f; // Assuming 40.0f is the desired offset; adjust as necessary
		m_offsetY = (float)interpolatedY - (m_height / (m_pixelsPerMeter * 4));// -11.5f; // Assuming 22.0f is the desired offset; adjust as necessary

		if (m_clampingOn) Clamp();
	}
}