#pragma once

#include "../../include/Camera/CameraManager.h"

namespace Engine
{
	CameraManager::CameraManager()
	{}

	void CameraManager::SetCurrentCamera(const size_t cameraEntityID)
	{
		m_currentCameraEntityID = cameraEntityID;
	}
}