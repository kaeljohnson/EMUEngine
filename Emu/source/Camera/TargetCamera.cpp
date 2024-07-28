#pragma once

#include "../../include/EngineConstants.h"
#include "../../include/MathUtil.h"

#include "../../include/Camera/TargetCamera.h"

namespace Engine
{
	TargetCamera::TargetCamera() : ptrCameraTarget(nullptr), Camera()
	{}

	// Bug: Camera is stuttery for certain values of m_smoothingFactor and bounds.
	void TargetCamera::Update(const float interpolation)
	{
		float targetX = Lerp(ptrCameraTarget->GetPhysicsBody()->GetCenterPrevX(), ptrCameraTarget->GetPhysicsBody()->GetCenterXInMeters(), interpolation);
		float targetY = Lerp(ptrCameraTarget->GetPhysicsBody()->GetCenterPrevY(), ptrCameraTarget->GetPhysicsBody()->GetCenterYInMeters(), interpolation);

		// Desired camera position based on the target's position
		float desiredCameraTopLeftX = targetX - (m_sizeInMeters.X / 2.0f);
		float desiredCameraTopLeftY = targetY - (m_sizeInMeters.Y / 2.0f);

		m_offset = Vector2D(desiredCameraTopLeftX, desiredCameraTopLeftY);

		// Clamp after adjustments if clamping is enabled
		if (m_clampingOn) Clamp();
	}	

	void TargetCamera::SetCameraTarget(SceneObject* target)
	{
		ptrCameraTarget = target;
	}
}