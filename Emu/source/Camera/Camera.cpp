#pragma once

#include "../../include/Camera/Camera.h"
#include "../../include/Logging/Logger.h"
#include "../../include/RendererManager.h"

namespace Engine
{
	Camera::Camera() : m_x(0), m_y(0), m_width(0), m_height(0), m_offsetX(0), m_offsetY(0), 
		currentScenesPixelsPerMeter(0), refSCALEX(0), refSCALEY(0), m_clampingOn(true), ptrCameraTarget(nullptr) {}

	Camera::Camera(float x, float y, const int w, const int h, const int levelWidth, const int levelHeight, const float SCALEX, const float SCALEY)
		: m_x(x), m_y(y), m_width(w), m_height(h), m_widthInMeters(0), m_heightInMeters(0), m_offsetX(0), m_offsetY(0),
		currentScenesPixelsPerMeter(0), refSCALEX(SCALEX), refSCALEY(SCALEY), m_clampingOn(true), ptrCameraTarget(nullptr) {}

	void Camera::SetCameraTarget(SceneObject* ptrTarget)
	{
		RendererManager::GetInstance()->SetCamera(this); // Set the camera to the renderer manager (this is the camera that will be used to render the scene
		ptrCameraTarget = ptrTarget;
	}

	void Camera::Frame(const int pixelsPerMeter, const float levelWidthInMeters, const float levelHeightInMeters, 
		const float screenWidth, const float screenHeight, const float scaleX, const float scaleY)
	{
		currentScenesPixelsPerMeter = pixelsPerMeter;

		currentScenesMapWidthPerMeter = levelWidthInMeters;
		currentScenesMapHeightPerMeter = levelHeightInMeters;

		refSCALEX = scaleX;
		refSCALEY = scaleY;

		m_width = screenWidth;
		m_height = screenHeight;

		m_widthInMeters = (float)m_width / (currentScenesPixelsPerMeter * refSCALEX);
		m_heightInMeters = (float)m_height / (currentScenesPixelsPerMeter * refSCALEY);

		ENGINE_INFO_D(std::to_string(currentScenesPixelsPerMeter));
		ENGINE_INFO_D(std::to_string(currentScenesMapWidthPerMeter));
		ENGINE_INFO_D(std::to_string(currentScenesMapHeightPerMeter));
		ENGINE_INFO_D(std::to_string(refSCALEX));
		ENGINE_INFO_D(std::to_string(refSCALEY));
		ENGINE_INFO_D("Camera width in pixels: " + std::to_string(m_width) + ", Camera height in pixels: " + std::to_string(m_height));
		ENGINE_INFO_D("Camera width in meters: " + std::to_string(m_widthInMeters) + ", Camera height in meters: " + std::to_string(m_heightInMeters));
	}

	void Camera::SetClampingOn(const bool clampingOn)
	{
		m_clampingOn = clampingOn;
	}

	void Camera::Clamp()
	{
		const float scalerX = currentScenesPixelsPerMeter * refSCALEX;
		const float scalerY = currentScenesPixelsPerMeter * refSCALEY;

		if (m_offsetX < 0) { m_offsetX = 0; }
		if (m_offsetX + m_widthInMeters > currentScenesMapWidthPerMeter) { m_offsetX = currentScenesMapWidthPerMeter - m_widthInMeters; }

		if (m_offsetY < 0) { m_offsetY = 0; }
		if (m_offsetY + m_heightInMeters > currentScenesMapHeightPerMeter) { m_offsetY = currentScenesMapHeightPerMeter - m_heightInMeters; }
	}


	void Camera::Update(double interpolation)
	{
		/*if (ptrCameraTarget == nullptr)
		{
			ENGINE_CRITICAL_D("Camera target is nullptr. Camera is static!");
			return;
		}*/

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
			m_offsetX += (desiredCameraTopLeftX - m_offsetX) * (smoothingFactor / refSCALEX);
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
			m_offsetY += (desiredCameraTopLeftY - m_offsetY) * (smoothingFactor / refSCALEY) ;
		}

		if (m_clampingOn) Clamp();
	}
}