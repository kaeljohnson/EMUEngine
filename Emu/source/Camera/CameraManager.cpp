#pragma once

#include "../../include/Camera/CameraManager.h"

namespace Engine
{
	CameraManager::CameraManager()
	{
		m_ptrCurrentCamera = nullptr;
	}

	void CameraManager::SetCurrentCamera(Camera* ptrCamera)
	{
		m_ptrCurrentCamera = ptrCamera;
	}
}