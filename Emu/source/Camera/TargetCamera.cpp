#pragma once

#include "../../include/EngineConstants.h"
#include "../../include/CommonFunctions.h"

#include "../../include/Camera/TargetCamera.h"

namespace Engine
{
	TargetCamera::TargetCamera() : ptrCameraTarget(nullptr), Camera()
	{}

	// Bug: Camera is stuttery for certain values of m_smoothingFactor and bounds.
	void TargetCamera::Update(const double interpolation)
	{
		double targetX = Lerp(ptrCameraTarget->GetPhysicsBody()->GetCenterPrevX(), ptrCameraTarget->GetPhysicsBody()->GetCenterXInMeters(), interpolation);
		double targetY = Lerp(ptrCameraTarget->GetPhysicsBody()->GetCenterPrevY(), ptrCameraTarget->GetPhysicsBody()->GetCenterYInMeters(), interpolation);

		// Desired camera position based on the target's position
		double desiredCameraTopLeftX = targetX - (m_widthInMeters / 2.0);
		double desiredCameraTopLeftY = targetY - (m_heightInMeters / 2.0);

		m_offsetX = desiredCameraTopLeftX;
		m_offsetY = desiredCameraTopLeftY;

		// Clamp after adjustments if clamping is enabled
		if (m_clampingOn) Clamp();
	}	

	void TargetCamera::SetCameraTarget(SceneObject* target)
	{
		ptrCameraTarget = target;
	}
}