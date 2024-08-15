#pragma once

#include "../../include/EngineConstants.h"
#include "../../include/MathUtil.h"

#include "../../include/Camera/TargetCamera.h"

namespace Engine
{
	TargetCamera::TargetCamera() : ptrCameraTarget(nullptr), Camera()
	{}

	// Bug: Camera is stuttery for certain values of m_smoothingFactor and bounds.
	void TargetCamera::Update(const double interpolation)
	{
		float targetX = Math::Lerp(ptrCameraTarget->GetPhysicsBody()->GetCenterPrevPosition().X, ptrCameraTarget->GetPhysicsBody()->GetCenterPosition().X, (float)interpolation);
		float targetY = Math::Lerp(ptrCameraTarget->GetPhysicsBody()->GetCenterPrevPosition().Y, ptrCameraTarget->GetPhysicsBody()->GetCenterPosition().Y, (float)interpolation);

		// Desired camera position based on the target's position
		float desiredCameraTopLeftX = targetX - (m_size.X / 2.0f);
		float desiredCameraTopLeftY = targetY - (m_size.Y / 2.0f);

		m_offset = Math::Vector2D(desiredCameraTopLeftX, desiredCameraTopLeftY);

		// Clamp after adjustments if clamping is enabled
		Clamp();
	}	

	void TargetCamera::SetCameraTarget(SceneObject* target)
	{
		ptrCameraTarget = target;
	}
}