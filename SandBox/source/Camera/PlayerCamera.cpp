#pragma once

#include <Engine.h>

#include "../../include/Camera/PlayerCamera.h"

PlayerCamera::PlayerCamera() : 
    m_smoothingFactor(0.001f),
    m_rightTargetScreenBound(1.0f), m_leftTargetScreenBound(0.0f), m_smoothingOn(true),
    m_topTargetScreenBound(0.25f), m_bottomTargetScreenBound(0.75f), m_lookAheadFactor(0.5f), m_lookAhead(0.0f)
{
    // Combine this whole class into player class.

	// Engine currently works by destroying all components on scene change.
	// Must re-add camera updater on scene play.
	Engine::EMU::GetInstance()->Scenes_AddComponent<Engine::CameraUpdater>("Level1", 1,
			[this](Engine::Entity entity) { Update(entity); });

	Engine::EMU::GetInstance()->Scenes_AddComponent<Engine::CameraUpdater>("Level2", 1,
		[this](Engine::Entity entity) { Update(entity); });
}

void PlayerCamera::Update(Engine::Entity entity)
{
    // CAMERA UPDATES

    const float interpFactor = Engine::Time::GetInterpolationFactor();

	const Math2D::Point2D<float> targetPrevPosition = Engine::EMU::GetInstance()->Transform_GetPrevPosition(entity);
	const Math2D::Point2D<float> targetPosition = Engine::EMU::GetInstance()->Transform_GetPosition(entity);

    Math2D::Point2D<float> targetPos = Math2D::Lerp(targetPrevPosition, targetPosition, interpFactor);
	Math2D::Point2D<float> cameraSize = Engine::EMU::GetInstance()->Camera_GetSize(entity);

	// Calculate the desired camera position based on the target's position
	float desiredCameraTopLeftX = targetPos.X - (cameraSize.X / 2.0f);
	float desiredCameraTopLeftY = targetPos.Y - (cameraSize.Y / 2.0f);

	// Set the camera offset to the desired position
	Engine::EMU::GetInstance()->Camera_SetOffset(entity, Math2D::Point2D<float>(desiredCameraTopLeftX, desiredCameraTopLeftY));

    return;
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