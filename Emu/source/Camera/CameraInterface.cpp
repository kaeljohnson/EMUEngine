#pragma once

#include "../../include/Camera/CameraInterface.h"
#include "../../include/Rendering/Screen.h"
#include "../../include/Components.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	CameraInterface::CameraInterface(ECS& refECS) : 
		m_refECS(refECS) {}

	void CameraInterface::SetPixelsPerUnit(Entity entity, const int pixelsPerUnit)
	{
		Camera* ptrCamera = m_refECS.GetComponent<Camera>(entity);

		ptrCamera->m_pixelsPerUnit = pixelsPerUnit;

		ptrCamera->m_size
			= Math2D::Point2D<float>(Screen::VIEWPORT_SIZE.X * ptrCamera->m_screenRatio.X / (ptrCamera->m_pixelsPerUnit * Screen::SCALE.X),
				Screen::VIEWPORT_SIZE.Y * ptrCamera->m_screenRatio.Y / (ptrCamera->m_pixelsPerUnit * Screen::SCALE.Y));
	}

	const size_t CameraInterface::GetPixelsPerUnit(Entity entity)
	{
		return m_refECS.GetComponent<Camera>(entity)->m_pixelsPerUnit;
	}

	const Math2D::Point2D<float> CameraInterface::GetSize(Entity entity)
	{
		return m_refECS.GetComponent<Camera>(entity)->m_size;
	}

	void CameraInterface::SetOffset(Entity entity, const Math2D::Point2D<float> offset)
	{
		m_refECS.GetComponent<Camera>(entity)->m_offset = offset;
	}

	const Math2D::Point2D<float> CameraInterface::GetOffset(Entity entity)
	{
		return m_refECS.GetComponent<Camera>(entity)->m_offset;
	}

	void CameraInterface::SetClampingOn(Entity entity, const bool clampingOn)
	{
		m_refECS.GetComponent<Camera>(entity)->m_clampingOn = clampingOn;
	}

	const bool CameraInterface::GetClampingOn(Entity entity)
	{
		return m_refECS.GetComponent<Camera>(entity)->m_clampingOn;
	}
}