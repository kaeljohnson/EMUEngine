#pragma once

#include "../../include/Camera/CameraInterface.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Rendering/Screen.h"
#include "../../include/Components.h"

namespace Engine
{
	CameraInterface::CameraInterface(ECS& refECS) : 
		m_refECS(refECS) {}

	void CameraInterface::SetPixelsPerUnit(Camera& refCamera, const int pixelsPerUnit)
	{
		refCamera.m_pixelsPerUnit = pixelsPerUnit;
		SetSize(refCamera);
	}

	void CameraInterface::SetSize(Camera& refCamera)
	{
		refCamera.m_size
			= Vector2D<float>(Screen::VIEWPORT_SIZE.X / (refCamera.m_pixelsPerUnit * Screen::SCALE.X), 
				Screen::VIEWPORT_SIZE.Y / (refCamera.m_pixelsPerUnit * Screen::SCALE.Y));
	}

	void CameraInterface::SetCameraPosition(Camera& refCamera, const Vector2D<float> offset)
	{
		refCamera.m_offset = offset;
	}

	void CameraInterface::SetClampingOn(Camera& refCamera, const bool clampingOn)
	{
		refCamera.m_clampingOn = clampingOn;
	}
}