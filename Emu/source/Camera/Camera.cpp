#pragma once

#include "../../include/Camera/Camera.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	Camera::Camera() : m_cameraType(STATIC_CAMERA), m_widthInMeters(0), m_heightInMeters(0), 
		m_offsetX(0), m_offsetY(0), m_mapBoundRight(0), m_mapBoundBottom(0), 
		SCALEX(0), SCALEY(0), m_clampingOn(true), ptrCameraTarget(nullptr), 
		m_horizontalScrollSpeed(0), m_verticalScrollSpeed(0) {}

	Camera::Camera(const CameraType cameraType) : m_cameraType(cameraType), 
		m_widthInMeters(0), m_heightInMeters(0), 
		m_offsetX(0), m_offsetY(0), m_mapBoundRight(0), m_mapBoundBottom(0),
		SCALEX(0), SCALEY(0), m_clampingOn(true), ptrCameraTarget(nullptr), 
		m_horizontalScrollSpeed(0), m_verticalScrollSpeed(0) {}

	void Camera::SetCameraTarget(SceneObject* ptrTarget)
	{
		ptrCameraTarget = ptrTarget;
	}

	void Camera::SetScrollingSpeeds(const float xScrollingSpeed, const float yScrollingSpeed)
	{
		m_horizontalScrollSpeed = xScrollingSpeed;
		m_verticalScrollSpeed = yScrollingSpeed;
	}

	void Camera::Frame(const int pixelsPerMeter, const float levelWidthInMeters, const float levelHeightInMeters,
		const float screenWidth, const float screenHeight, const float scaleX, const float scaleY)
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


	void Camera::Update(double interpolation)
	{
		if (m_cameraType == STATIC_CAMERA) return; // Static camera does not move.

		if (m_cameraType == SCROLLING)
		{
			m_offsetX += m_horizontalScrollSpeed;
			m_offsetY += m_verticalScrollSpeed;
			return;
		}

		// Calculate the interpolated position of the target
		double targetX = (ptrCameraTarget->GetPhysicsBody()->GetCenterPrevX() * (1.0f - interpolation)) +
			(ptrCameraTarget->GetPhysicsBody()->GetCenterXInMeters() * interpolation);
		double targetY = (ptrCameraTarget->GetPhysicsBody()->GetCenterPrevY() * (1.0f - interpolation)) +
			(ptrCameraTarget->GetPhysicsBody()->GetCenterYInMeters() * interpolation);

		// Desired camera position based on the target's position
		float desiredCameraTopLeftX = (float)targetX - (m_widthInMeters / 2.0f);
		float desiredCameraTopLeftY = (float)targetY - (m_heightInMeters / 2.0f);

		// Smoothly update the camera's position towards the desired position
		float smoothingFactor = 0.0005f; // Adjust this value to control the camera's smoothing speed

		if (targetX > m_offsetX + ((m_widthInMeters) * 0.7f))
		{
			m_offsetX = (float)targetX - ((m_widthInMeters) * 0.7f);
		}
		else if (targetX < m_offsetX + ((m_widthInMeters) * 0.3f))
		{
			m_offsetX = (float)targetX - ((m_widthInMeters) * 0.3f);
		}
		else
		{
			m_offsetX += (desiredCameraTopLeftX - m_offsetX) * (smoothingFactor / SCALEX);
		}

		if (targetY > m_offsetY + ((m_heightInMeters) * 0.7f))
		{
			m_offsetY = (float)targetY - ((m_heightInMeters) * 0.7f);
		}
		else if (targetY < m_offsetY + ((m_heightInMeters) * 0.3f))
		{
			m_offsetY = (float)targetY - ((m_heightInMeters) * 0.3f);
		}
		else
		{
			m_offsetY += (desiredCameraTopLeftY - m_offsetY) * (smoothingFactor / SCALEY);
		}

		if (m_clampingOn) Clamp();
	}
}