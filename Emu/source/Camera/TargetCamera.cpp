#pragma once

#include "../../include/EngineConstants.h"
#include "../../include/MathUtil.h"

#include "../../include/Camera/TargetCamera.h"

#include "../../include/ECS/EntityManager.h"

#include "../../include/Transform.h"

//namespace Engine
//{
//	TargetCamera::TargetCamera(const size_t targetEntityID) : m_cameraTargetEntityID(targetEntityID), Camera()
//	{}
//
//	// Bug: Camera is stuttery for certain values of m_smoothingFactor and bounds.
//	void TargetCamera::Update(const double interpolation)
//	{
//		Engine::Transform* ptrCameraTarget = 
//			EntityManager::GetComponentManager<Engine::Transform>().GetComponent(m_cameraTargetEntityID);
//
//		float targetX = Lerp(ptrCameraTarget->PrevPosition.X, ptrCameraTarget->Position.X, (float)interpolation);
//		float targetY = Lerp(ptrCameraTarget->PrevPosition.Y, ptrCameraTarget->Position.Y, (float)interpolation);
//
//		// Desired camera position based on the target's position
//		float desiredCameraTopLeftX = targetX - (m_size.X / 2.0f);
//		float desiredCameraTopLeftY = targetY - (m_size.Y / 2.0f);
//
//		m_offset = Vector2D(desiredCameraTopLeftX, desiredCameraTopLeftY);
//
//		// Clamp after adjustments if clamping is enabled
//		if (m_clampingOn) Clamp();
//	}	
//
//	void TargetCamera::SetCameraTarget(const size_t targetEntityID)
//	{
//		m_cameraTargetEntityID = targetEntityID;
//	}
//}