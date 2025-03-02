#pragma once

#include <Engine.h>

#include "../../include/Camera/PlayerCamera.h"

PlayerCamera::PlayerCamera(Engine::Entity* ptrEntity) : 
    m_ptrEntity(ptrEntity), m_smoothingFactor(0.001f),
    m_rightTargetScreenBound(1.0f), m_leftTargetScreenBound(0.0f), m_smoothingOn(true),
    m_topTargetScreenBound(0.25f), m_bottomTargetScreenBound(0.75f), m_lookAheadFactor(0.5f), m_lookAhead(0.0f)
{
    // Combine this whole class into player class.

	Engine::EMU::GetInstance()->AddComponent<Engine::Camera>(ptrEntity);
    // Engine::EMU::GetInstance()->IECS().AddComponent<Engine::Updatable>(ptrEntity, [this]() { Update(); });

    // playerCamera->SetPixelsPerUnit(32);
	Engine::EMU::GetInstance()->ICAMERA().SetPixelsPerUnit(m_ptrEntity, 32);
}

void PlayerCamera::Update()
{
	// Engine::Transform* ptrCameraTarget = Engine::EMU::GetInstance()->IECS().GetComponentManager<Engine::Transform>().GetComponent(m_ptrEntity);
	Engine::TransformInterface& refTransformInterface = Engine::EMU::GetInstance()->ITRANSFORMS();
	// Engine::Camera* ptrCamera = Engine::EMU::GetInstance()->IECS().GetComponentManager<Engine::Camera>().GetComponent(m_ptrEntity);

	Engine::Vector2D<float> targetPrevPosition = refTransformInterface.GetPrevPosition(m_ptrEntity);
	Engine::Vector2D<float> targetPosition = refTransformInterface.GetPosition(m_ptrEntity);
    float targetX = Engine::Lerp(targetPrevPosition.X, targetPosition.X, Engine::Time::GetInterpolationFactor());
    float targetY = Engine::Lerp(targetPrevPosition.Y, targetPosition.Y, Engine::Time::GetInterpolationFactor());

	Engine::CameraInterface& refCameraInterface = Engine::EMU::GetInstance()->ICAMERA();
    const Engine::Vector2D<float> cameraSize = refCameraInterface.GetSize(m_ptrEntity);
	Engine::EMU::GetInstance()->ICAMERA().SetOffsets(m_ptrEntity, 
        Engine::Vector2D<float>(targetX - (cameraSize.X / 2.0f), targetY - (cameraSize.Y / 2.0f)));

    // if (ptrCamera->m_clampingOn) ptrCamera->Clamp(); Engine willhandle clamping.

    return;

    // DEAD CODE FOR NOW

    float desiredLookAhead = refTransformInterface.GetDirectionFacing(m_ptrEntity) * m_lookAheadFactor;

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
    float desiredCameraTopLeftX = targetX - (cameraSize.X / 2.0f);
    float desiredCameraTopLeftY = targetY - (cameraSize.Y / 2.0f);

    const Engine::Vector2D<float> cameraOffset = refCameraInterface.GetOffset(m_ptrEntity);

	refCameraInterface.SetOffsets(m_ptrEntity, Engine::Vector2D<float>(desiredCameraTopLeftX - cameraOffset.X, cameraOffset.Y));

    if (targetY > cameraOffset.Y + ((cameraSize.Y) * m_bottomTargetScreenBound))
    {
		refCameraInterface.SetOffsets(m_ptrEntity, Engine::Vector2D<float>(cameraOffset.X, targetY - ((cameraSize.Y) * m_bottomTargetScreenBound)));
    }
    else if (targetY < cameraOffset.Y + ((cameraSize.Y)*m_topTargetScreenBound))
    {
		refCameraInterface.SetOffsets(m_ptrEntity, Engine::Vector2D<float>(cameraOffset.X, targetY - ((cameraSize.Y) * m_topTargetScreenBound)));
    }
    else if (m_smoothingOn)
    {
		// Smoothly move the camera towards the desired position
        refCameraInterface.SetOffsets(m_ptrEntity, 
            Engine::Vector2D<float>(cameraOffset.X, (desiredCameraTopLeftY - cameraOffset.Y) * (m_smoothingFactor / Engine::Screen::GetScale().Y)));
    }
    else
    {
		refCameraInterface.SetOffsets(m_ptrEntity, Engine::Vector2D<float>(cameraOffset.X, desiredCameraTopLeftY));
    }

    // if (ptrCamera->m_clampingOn) ptrCamera->Clamp();
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