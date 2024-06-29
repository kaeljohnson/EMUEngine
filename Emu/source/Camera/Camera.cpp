#pragma once

#include "../../include/Camera/Camera.h"
#include "../../include/Logging/Logger.h"


namespace Engine
{
	Camera::Camera() : m_x(0), m_y(0), m_width(0), m_height(0), m_levelWidth(0), m_levelHeight(0), m_offsetX(0), m_offsetY(0), m_pixelsPerMeter(0), refSCALE(0), m_clampingOn(true), ptrCameraTarget(nullptr) {}

	Camera::Camera(float x, float y, const int w, const int h, const int levelWidth, const int levelHeight, const float SCALE)
		: m_x(x), m_y(y), m_width(w), m_height(h), m_offsetX(0), m_offsetY(0), m_levelWidth(levelWidth), m_levelHeight(levelHeight), m_pixelsPerMeter(0), refSCALE(SCALE), m_clampingOn(true), ptrCameraTarget(nullptr) {}

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

	void Camera::Update(double interpolation)
	{
		if (ptrCameraTarget == nullptr)
		{
			ENGINE_CRITICAL_D("Camera target is nullptr. Camera is static!");
			return;
		}

		const float denom = (m_pixelsPerMeter * 4.0f);
		const float widthInMeters = (float)m_width / (m_pixelsPerMeter * refSCALE);
		const float heightInMeters = (float)m_height / (m_pixelsPerMeter * refSCALE);

		// Calculate the interpolated position of the target
		double targetX = (ptrCameraTarget->GetPhysicsBody()->GetCenterPrevX() * (1.0f - interpolation)) +
			(ptrCameraTarget->GetPhysicsBody()->GetCenterXInMeters() * interpolation);
		double targetY = (ptrCameraTarget->GetPhysicsBody()->GetCenterPrevY() * (1.0f - interpolation)) +
			(ptrCameraTarget->GetPhysicsBody()->GetCenterYInMeters() * interpolation);

		// Desired camera position based on the target's position
		float desiredCameraTopLeftX = (float)targetX - (widthInMeters / 2.0f);
		float desiredCameraTopLeftY = (float)targetY - (heightInMeters / 2.0f);

		// m_offsetX = desiredCameraTopLeftX;
		// m_offsetY = desiredCameraTopLeftY;

		// Smoothly update the camera's position towards the desired position
		float smoothingFactor = 0.001f; // Adjust this value to control the camera's smoothing speed
		// m_offsetX += (desiredX - m_offsetX) * smoothingFactor;
		// m_offsetY += (desiredY - m_offsetY) * smoothingFactor;

		if (targetX > m_offsetX + ((widthInMeters) * 0.7f))
		{
			m_offsetX = (float)targetX - ((widthInMeters) * 0.7f);
			ENGINE_INFO_D("1");
		}
		else if (targetX < m_offsetX + ((widthInMeters) * 0.3f))
		{
			m_offsetX = (float)targetX - ((widthInMeters) * 0.3f);
			ENGINE_INFO_D("2");
		}
		else
		{
			m_offsetX += (desiredCameraTopLeftX - m_offsetX) * smoothingFactor;
			ENGINE_INFO_D("3");
		}

		if (targetY > m_offsetY + ((heightInMeters) * 0.7f))
		{
			m_offsetY = (float)targetY - ((heightInMeters) * 0.7f);
			ENGINE_INFO_D("4");
		}
		else if (targetY < m_offsetY + ((heightInMeters) * 0.3f))
		{
			m_offsetY = (float)targetY - ((heightInMeters) * 0.3f);
			ENGINE_INFO_D("5");
		}
		else
		{
			m_offsetY += (desiredCameraTopLeftY - m_offsetY) * smoothingFactor;
			ENGINE_INFO_D("6");
		}

		if (m_clampingOn) Clamp();
	}
}