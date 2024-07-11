#pragma once

#include "../../include/Camera/PlayerCamera.h"

PlayerCamera::PlayerCamera() : m_smoothingFactor(0.001f),
m_rightTargetScreenBound(1.0f), m_leftTargetScreenBound(0.0f), m_smoothingOn(false),
m_topTargetScreenBound(0.25f), m_bottomTargetScreenBound(0.75f), m_lookAheadFactor(1.0f), m_lookAhead(0.0f), TargetCamera()
{
}

//void PlayerCamera::Update(const double interpolation)
//{
//	double targetX = Engine::Lerp(ptrCameraTarget->GetPhysicsBody()->GetCenterPrevX(), ptrCameraTarget->GetPhysicsBody()->GetCenterXInMeters(), interpolation);
//	double targetY = Engine::Lerp(ptrCameraTarget->GetPhysicsBody()->GetCenterPrevY(), ptrCameraTarget->GetPhysicsBody()->GetCenterYInMeters(), interpolation);
//
//	// bool centerPlayer = (targetX <= m_offsetX + (m_widthInMeters / 2.0f) && ptrCameraTarget->refKeyStates.at(Engine::D_KEY_DOWN)) || 
//	// 	(targetX >= m_offsetX + (m_widthInMeters / 2.0f) && (ptrCameraTarget->refKeyStates.at(Engine::A_KEY_DOWN)));
//
//	double desiredLookAhead = ptrCameraTarget->DirectionFacing * m_lookAheadFactor;
//
//	m_lookAhead = Engine::Lerp(m_lookAhead, desiredLookAhead, interpolation * 0.001);
//
//	targetX += m_lookAhead;
//
//	// Desired camera position based on the target's position
//	double desiredCameraTopLeftX = (float)targetX - (m_widthInMeters / 2.0);
//	double desiredCameraTopLeftY = (float)targetY - (m_heightInMeters / 2.0);
//
//	m_offsetX += (desiredCameraTopLeftX - m_offsetX); // * (m_smoothingFactor / SCALEX);
//	// m_offsetY += (desiredCameraTopLeftY - m_offsetY) * (0.001f / SCALEY);
//	
//	if (targetY > m_offsetY + ((m_heightInMeters) * m_bottomTargetScreenBound))
//	{
//		m_offsetY = (float)targetY - ((m_heightInMeters) * m_bottomTargetScreenBound);
//	}
//	else if (targetY < m_offsetY + ((m_heightInMeters) * m_topTargetScreenBound))
//	{
//		m_offsetY = (float)targetY - ((m_heightInMeters) * m_topTargetScreenBound);
//	}
//	else if (m_smoothingOn)
//	{
//		m_offsetY += (desiredCameraTopLeftY - m_offsetY) * (m_smoothingFactor / SCALEY);
//	}
//	else
//	{
//		m_offsetY = desiredCameraTopLeftY;
//	}
//
//	if (m_clampingOn) Clamp();
//}

void PlayerCamera::Update(const double interpolation)
{
    double targetX = Engine::Lerp(ptrCameraTarget->GetPhysicsBody()->GetCenterPrevX(), ptrCameraTarget->GetPhysicsBody()->GetCenterXInMeters(), interpolation);
    double targetY = Engine::Lerp(ptrCameraTarget->GetPhysicsBody()->GetCenterPrevY(), ptrCameraTarget->GetPhysicsBody()->GetCenterYInMeters(), interpolation);

    double desiredLookAhead = ptrCameraTarget->DirectionFacing * m_lookAheadFactor;

     // Adjust this value as needed

    // Calculate the absolute difference between the current look-ahead and the desired look-ahead
    double lookAheadDifference = std::abs(m_lookAhead - desiredLookAhead);

    // If the look-ahead difference is under the threshold, snap the look-ahead to the player's position
    if (lookAheadDifference < m_minLookAheadDistance)
    {
        m_lookAhead = desiredLookAhead; // This effectively snaps the camera to the player's position without the look-ahead
    }
    else
    {
        m_lookAhead = Engine::Lerp(m_lookAhead, desiredLookAhead, interpolation * 0.002);
    }

    targetX += m_lookAhead;

    // Desired camera position based on the target's position
    double desiredCameraTopLeftX = targetX - (m_widthInMeters / 2.0);
    double desiredCameraTopLeftY = targetY - (m_heightInMeters / 2.0);

    m_offsetX += (desiredCameraTopLeftX - m_offsetX); // Smooth or snap camera movement
    // m_offsetY += (desiredCameraTopLeftY - m_offsetY) * (0.001f / SCALEY); // Adjust as needed for Y-axis

    if (targetY > m_offsetY + ((m_heightInMeters)*m_bottomTargetScreenBound))
    {
        m_offsetY = targetY - ((m_heightInMeters)*m_bottomTargetScreenBound);
    }
    else if (targetY < m_offsetY + ((m_heightInMeters)*m_topTargetScreenBound))
    {
        m_offsetY = targetY - ((m_heightInMeters)*m_topTargetScreenBound);
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