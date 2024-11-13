#pragma once

#include <Engine.h>

#include "../../include/Camera/PlayerCamera.h"

PlayerCamera::PlayerCamera(Engine::Entity* ptrEntity, Engine::Entity* ptrPlayerEntity) : 
    m_ptrEntity(ptrEntity), m_ptrCameraTargetEntity(ptrPlayerEntity), m_smoothingFactor(0.001f),
    m_rightTargetScreenBound(1.0f), m_leftTargetScreenBound(0.0f), m_smoothingOn(true),
    m_topTargetScreenBound(0.25f), m_bottomTargetScreenBound(0.75f), m_lookAheadFactor(0.5f), m_lookAhead(0.0f)
{
	Engine::ECS::GetComponentManager<Engine::Camera>().AddComponent(ptrEntity);
    Engine::ECS::GetComponentManager<Engine::Updatable>().AddComponent(ptrEntity, [this]() { Update(); });

	Engine::Camera* playerCamera = Engine::ECS::GetComponentManager<Engine::Camera>().GetComponent(m_ptrEntity);
    playerCamera->SetPixelsPerUnit(32);
}

void PlayerCamera::Update()
{
	Engine::Transform* ptrCameraTarget = Engine::ECS::GetComponentManager<Engine::Transform>().GetComponent(m_ptrCameraTargetEntity);
	Engine::Camera* ptrCamera = Engine::ECS::GetComponentManager<Engine::Camera>().GetComponent(m_ptrEntity);

	// Need something like Time::GetInterpolationFactor=() to get the time between frames

    float targetX = Engine::Lerp(ptrCameraTarget->PrevPosition.X, ptrCameraTarget->Position.X, Engine::Time::GetInterpolationFactor());
    float targetY = Engine::Lerp(ptrCameraTarget->PrevPosition.Y, ptrCameraTarget->Position.Y, Engine::Time::GetInterpolationFactor());

    float desiredLookAhead = ptrCameraTarget->DirectionFacing * m_lookAheadFactor;

    // Calculate the difference between the current and desired look-ahead
    float lookAheadDifference = desiredLookAhead - m_lookAhead;

    // Calculate the step to move towards the desired look-ahead, ensuring we don't overshoot
    m_lookAheadChangeSpeed = 0.005f / Engine::Screen::GetScale().X;

    float lookAheadStep = std::min(std::abs(lookAheadDifference), m_lookAheadChangeSpeed * Engine::Time::GetInterpolationFactor());
    lookAheadStep *= (lookAheadDifference > 0) ? 1 : -1; // Ensure the step has the correct direction

    // Update the look-ahead
    m_lookAhead += lookAheadStep;

    targetX += m_lookAhead;

    // Desired camera position based on the target's position
    float desiredCameraTopLeftX = targetX - (ptrCamera->m_size.X / 2.0f);
    float desiredCameraTopLeftY = targetY - (ptrCamera->m_size.Y / 2.0f);

    ptrCamera->m_offset.X += (desiredCameraTopLeftX - ptrCamera->m_offset.X);

    if (targetY > ptrCamera->m_offset.Y + ((ptrCamera->m_size.Y) * m_bottomTargetScreenBound))
    {
        ptrCamera->m_offset.Y = targetY - ((ptrCamera->m_size.Y) * m_bottomTargetScreenBound);
    }
    else if (targetY < ptrCamera->m_offset.Y + ((ptrCamera->m_size.Y)*m_topTargetScreenBound))
    {
        ptrCamera->m_offset.Y = targetY - ((ptrCamera->m_size.Y)*m_topTargetScreenBound);
    }
    else if (m_smoothingOn)
    {
        ptrCamera->m_offset.Y += (desiredCameraTopLeftY - ptrCamera->m_offset.Y) * (m_smoothingFactor / Engine::Screen::GetScale().Y);
    }
    else
    {
        ptrCamera->m_offset.Y = desiredCameraTopLeftY;
    }

    if (ptrCamera->m_clampingOn) ptrCamera->Clamp();
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