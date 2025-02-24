#pragma once

#include "../../include/Camera/CameraInterface.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Rendering/Screen.h"
#include "../../include/Components.h"

namespace Engine
{
	CameraInterface::CameraInterface(ECS& refECS) : 
		m_refECS(refECS) {}

	Camera* CameraInterface::GetCamera(Entity* ptrEntity)
	{
		return m_refECS.GetComponent<Camera>(ptrEntity);
	}

	void CameraInterface::SetPixelsPerUnit(Entity* ptrEntity, const int pixelsPerUnit)
	{
		GetCamera(ptrEntity)->m_pixelsPerUnit = pixelsPerUnit;
		SetSize(ptrEntity);
	}

	void CameraInterface::SetSize(Entity* ptrEntity)
	{
		Camera* ptrCamera = GetCamera(ptrEntity);
		ptrCamera->m_size
			= Vector2D<float>(Screen::VIEWPORT_SIZE.X / (ptrCamera->m_pixelsPerUnit * Screen::SCALE.X), 
				Screen::VIEWPORT_SIZE.Y / (ptrCamera->m_pixelsPerUnit * Screen::SCALE.Y));
	}

	void CameraInterface::SetCameraPosition(Entity* ptrEntity, const Vector2D<float> offset)
	{
		GetCamera(ptrEntity)->m_offset = offset;
	}

	void CameraInterface::SetClampingOn(Entity* ptrEntity, const bool clampingOn)
	{
		GetCamera(ptrEntity)->m_clampingOn = clampingOn;
	}
}