#pragma once

#include "../../include/Camera/PlayerCamera.h"

PlayerCamera::PlayerCamera() : m_smoothingFactor(0.001f),
m_rightTargetScreenBound(1.0f), m_leftTargetScreenBound(0.0f), m_smoothingOn(false),
m_topTargetScreenBound(0.25f), m_bottomTargetScreenBound(0.75f), m_lookAheadFactor(1.0f), m_lookAhead(0.0f), TargetCamera()
{
}

void PlayerCamera::Update(const double interpolation)
{
    double targetX = Engine::Lerp(ptrCameraTarget->GetPhysicsBody()->GetCenterPrevX(), ptrCameraTarget->GetPhysicsBody()->GetCenterXInMeters(), interpolation);
    double targetY = Engine::Lerp(ptrCameraTarget->GetPhysicsBody()->GetCenterPrevY(), ptrCameraTarget->GetPhysicsBody()->GetCenterYInMeters(), interpolation);

    double desiredLookAhead = ptrCameraTarget->DirectionFacing * m_lookAheadFactor;

    // Calculate the difference between the current and desired look-ahead
    double lookAheadDifference = desiredLookAhead - m_lookAhead;

    // Calculate the step to move towards the desired look-ahead, ensuring we don't overshoot
    ENGINE_CRITICAL_D(std::to_string(m_screenWidth));
    m_lookAheadChangeSpeed = 12.8f / m_screenWidth;
    double lookAheadStep = std::min(std::abs(lookAheadDifference), m_lookAheadChangeSpeed * interpolation);
    lookAheadStep *= (lookAheadDifference > 0) ? 1 : -1; // Ensure the step has the correct direction

    // Update the look-ahead
    m_lookAhead += lookAheadStep;

    targetX += m_lookAhead;

    // Desired camera position based on the target's position
    double desiredCameraTopLeftX = targetX - (m_widthInMeters / 2.0);
    double desiredCameraTopLeftY = targetY - (m_heightInMeters / 2.0);

    m_offsetX += (desiredCameraTopLeftX - m_offsetX);

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