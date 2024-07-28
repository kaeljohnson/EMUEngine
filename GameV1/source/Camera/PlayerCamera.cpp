#pragma once

#include "../../include/Camera/PlayerCamera.h"

PlayerCamera::PlayerCamera() : m_smoothingFactor(0.001f),
m_rightTargetScreenBound(1.0f), m_leftTargetScreenBound(0.0f), m_smoothingOn(true),
m_topTargetScreenBound(0.25f), m_bottomTargetScreenBound(0.75f), m_lookAheadFactor(0.5f), m_lookAhead(0.0f), TargetCamera()
{
}

void PlayerCamera::Update(const float interpolation)
{
    float targetX = Engine::Lerp(ptrCameraTarget->GetPhysicsBody()->GetCenterPrevX(), ptrCameraTarget->GetPhysicsBody()->GetCenterXInMeters(), interpolation);
    float targetY = Engine::Lerp(ptrCameraTarget->GetPhysicsBody()->GetCenterPrevY(), ptrCameraTarget->GetPhysicsBody()->GetCenterYInMeters(), interpolation);

    float desiredLookAhead = ptrCameraTarget->DirectionFacing * m_lookAheadFactor;

    // Calculate the difference between the current and desired look-ahead
    float lookAheadDifference = desiredLookAhead - m_lookAhead;

    // Calculate the step to move towards the desired look-ahead, ensuring we don't overshoot
    m_lookAheadChangeSpeed = 0.005f / refScale.X;

    float lookAheadStep = std::min(std::abs(lookAheadDifference), m_lookAheadChangeSpeed * interpolation);
    lookAheadStep *= (lookAheadDifference > 0) ? 1 : -1; // Ensure the step has the correct direction

    // Update the look-ahead
    m_lookAhead += lookAheadStep;

    targetX += m_lookAhead;

    // Desired camera position based on the target's position
    float desiredCameraTopLeftX = targetX - (m_sizeInMeters.X / 2.0f);
    float desiredCameraTopLeftY = targetY - (m_sizeInMeters.Y / 2.0f);

    m_offset.X += (desiredCameraTopLeftX - m_offset.X);

    if (targetY > m_offset.Y + ((m_sizeInMeters.Y) * m_bottomTargetScreenBound))
    {
        m_offset.Y = targetY - ((m_sizeInMeters.Y) * m_bottomTargetScreenBound);
    }
    else if (targetY < m_offset.Y + ((m_sizeInMeters.Y)*m_topTargetScreenBound))
    {
        m_offset.Y = targetY - ((m_sizeInMeters.Y)*m_topTargetScreenBound);
    }
    else if (m_smoothingOn)
    {
        m_offset.Y += (desiredCameraTopLeftY - m_offset.Y) * (m_smoothingFactor / refScale.Y);
    }
    else
    {
        m_offset.Y = desiredCameraTopLeftY;
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