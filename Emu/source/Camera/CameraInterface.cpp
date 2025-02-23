#pragma once

#include "../../include/Camera/CameraInterface.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Rendering/Screen.h"
#include "../../include/Components.h"

namespace Engine
{
	CameraInterface::CameraInterface(ECS& refECS) : 
		m_refECS(refECS) {}

	void CameraInterface::SetPixelsPerUnit(Camera* ptrCamera, const int pixelsPerUnit)
	{
		ptrCamera->m_pixelsPerUnit = pixelsPerUnit;
		SetSize(ptrCamera);
	}

	void CameraInterface::SetSize(Camera* ptrCamera)
	{
		ptrCamera->m_size
			= Vector2D<float>(Screen::VIEWPORT_SIZE.X / (ptrCamera->m_pixelsPerUnit * Screen::SCALE.X), 
				Screen::VIEWPORT_SIZE.Y / (ptrCamera->m_pixelsPerUnit * Screen::SCALE.Y));
	}

	void CameraInterface::SetCameraPosition(Camera* ptrCamera, const Vector2D<float> offset)
	{
		ptrCamera->m_offset = offset;
	}

	void CameraInterface::SetClampingOn(Camera* ptrCamera, const bool clampingOn)
	{
		ptrCamera->m_clampingOn = clampingOn;
	}
}