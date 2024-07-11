#pragma once

#include "../../include/Camera/PlayerCamera.h"

PlayerCamera::PlayerCamera() : m_smoothingFactor(0.001f),
m_rightTargetScreenBound(1.0f), m_leftTargetScreenBound(0.0f), m_smoothingOn(false),
m_topTargetScreenBound(0.0f), m_bottomTargetScreenBound(1.0f), m_lookAheadFactor(0.0f), m_lookAhead(0.0f), TargetCamera()
{
}

void PlayerCamera::Update(const double interpolation)
{
	double targetX = Engine::Lerp(ptrCameraTarget->GetPhysicsBody()->GetCenterPrevX(), ptrCameraTarget->GetPhysicsBody()->GetCenterXInMeters(), interpolation);
	double targetY = Engine::Lerp(ptrCameraTarget->GetPhysicsBody()->GetCenterPrevY(), ptrCameraTarget->GetPhysicsBody()->GetCenterYInMeters(), interpolation);

	// bool centerPlayer = (targetX <= m_offsetX + (m_widthInMeters / 2.0f) && ptrCameraTarget->refKeyStates.at(Engine::D_KEY_DOWN)) || 
	// 	(targetX >= m_offsetX + (m_widthInMeters / 2.0f) && (ptrCameraTarget->refKeyStates.at(Engine::A_KEY_DOWN)));

	double desiredLookAhead = ptrCameraTarget->DirectionFacing * m_lookAheadFactor;

	m_lookAhead = Engine::Lerp(m_lookAhead, desiredLookAhead, interpolation * 0.001);

	targetX += m_lookAhead;

	// Desired camera position based on the target's position
	double desiredCameraTopLeftX = (float)targetX - (m_widthInMeters / 2.0);
	double desiredCameraTopLeftY = (float)targetY - (m_heightInMeters / 2.0);

	m_offsetX += (desiredCameraTopLeftX - m_offsetX); // * (m_smoothingFactor / SCALEX);
	// m_offsetY += (desiredCameraTopLeftY - m_offsetY) * (0.001f / SCALEY);
	
	if (targetY > m_offsetY + ((m_heightInMeters) * m_bottomTargetScreenBound))
	{
		m_offsetY = (float)targetY - ((m_heightInMeters) * m_bottomTargetScreenBound);
	}
	else if (targetY < m_offsetY + ((m_heightInMeters) * m_topTargetScreenBound))
	{
		m_offsetY = (float)targetY - ((m_heightInMeters) * m_topTargetScreenBound);
	}
	else if (m_smoothingOn)
	{
		m_offsetY += (desiredCameraTopLeftY - m_offsetY) * (m_smoothingFactor / SCALEY);
	}
	else
	{
		m_offsetY = desiredCameraTopLeftY;
	}

	if (m_clampingOn) Clamp();
}

void PlayerCamera::SetLookAheadFactor(const float lookAheadFactor)
{
	m_lookAheadFactor = lookAheadFactor;
}

void PlayerCamera::SetTargetSmoothingFactor(const float smoothingFactor)
{
	m_smoothingFactor = smoothingFactor;
	m_smoothingOn = true;
}

void PlayerCamera::SetRightTargetScreenBound(const float screenBound)
{
	m_rightTargetScreenBound = screenBound;
}

void PlayerCamera::SetLeftTargetScreenBound(const float screenBound)
{
	m_leftTargetScreenBound = screenBound;
}

void PlayerCamera::SetTopTargetScreenBound(const float screenBound)
{
	m_topTargetScreenBound = screenBound;
}

void PlayerCamera::SetBottomTargetScreenBound(const float screenBound)
{
	m_bottomTargetScreenBound = screenBound;
}