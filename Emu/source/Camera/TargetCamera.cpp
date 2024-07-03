#pragma once

#include "../../include/Camera/TargetCamera.h"

namespace Engine
{
	TargetCamera::TargetCamera() :  ptrCameraTarget(nullptr), m_smoothingFactor(0.001f), 
		m_rightTargetScreenBound(1.0f), m_leftTargetScreenBound(0.0f), 
		m_topTargetScreenBound(0.0f), m_bottomTargetScreenBound(1.0f), Camera()
	{}

	// Bug: Camera is stuttery for certain values of m_smoothingFactor and bounds.
	void TargetCamera::Update(const double interpolation)
	{
		double targetX = (ptrCameraTarget->GetPhysicsBody()->GetCenterPrevX() * (1.0f - interpolation)) +
			(ptrCameraTarget->GetPhysicsBody()->GetCenterXInMeters() * interpolation);
		double targetY = (ptrCameraTarget->GetPhysicsBody()->GetCenterPrevY() * (1.0f - interpolation)) +
			(ptrCameraTarget->GetPhysicsBody()->GetCenterYInMeters() * interpolation);

		// Desired camera position based on the target's position
		float desiredCameraTopLeftX = (float)targetX - (m_widthInMeters / 2.0f);
		float desiredCameraTopLeftY = (float)targetY - (m_heightInMeters / 2.0f);


		if (targetX > m_offsetX + ((m_widthInMeters)*m_rightTargetScreenBound))
		{
			m_offsetX = (float)targetX - ((m_widthInMeters)*m_rightTargetScreenBound);
		}
		else if (targetX < m_offsetX + ((m_widthInMeters)*m_leftTargetScreenBound))
		{
			m_offsetX = (float)targetX - ((m_widthInMeters)*m_leftTargetScreenBound);
		}
		else
		{
			m_offsetX += (desiredCameraTopLeftX - m_offsetX) * (m_smoothingFactor / SCALEX);
		}

		if (targetY > m_offsetY + ((m_heightInMeters)*m_bottomTargetScreenBound))
		{
			m_offsetY = (float)targetY - ((m_heightInMeters)*m_bottomTargetScreenBound);
		}
		else if (targetY < m_offsetY + ((m_heightInMeters)*m_topTargetScreenBound))
		{
			m_offsetY = (float)targetY - ((m_heightInMeters)*m_topTargetScreenBound);
		}
		else
		{
			m_offsetY += (desiredCameraTopLeftY - m_offsetY) * (m_smoothingFactor / SCALEY);
		}

		if (m_clampingOn) Clamp();
	}

	void TargetCamera::SetCameraTarget(SceneObject* ptrTarget)
	{
		ptrCameraTarget = ptrTarget;
	}

	void TargetCamera::SetTargetSmoothingFactor(const float smoothingFactor)
	{
		m_smoothingFactor = smoothingFactor;
	}

	void TargetCamera::SetRightTargetScreenBound(const float screenBound)
	{
		m_rightTargetScreenBound = screenBound;
	}

	void TargetCamera::SetLeftTargetScreenBound(const float screenBound)
	{
		m_leftTargetScreenBound = screenBound;
	}

	void TargetCamera::SetTopTargetScreenBound(const float screenBound)
	{
		m_topTargetScreenBound = screenBound;
	}

	void TargetCamera::SetBottomTargetScreenBound(const float screenBound)
	{
		m_bottomTargetScreenBound = screenBound;
	}
}