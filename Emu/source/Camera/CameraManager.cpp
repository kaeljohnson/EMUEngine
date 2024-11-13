#pragma once

#include "../../include/Camera/CameraManager.h"

namespace Engine
{
	CameraManager::CameraManager()
	{}

	void CameraManager::SetCurrentCamera(Entity* ptrCameraEntity)
	{
		m_ptrCurrentCameraEntity = ptrCameraEntity;
	}
}