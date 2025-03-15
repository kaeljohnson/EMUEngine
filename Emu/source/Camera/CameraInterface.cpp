#pragma once

#include "../../include/Camera/CameraInterface.h"
#include "../../include/Rendering/Screen.h"
#include "../../include/Components.h"

namespace Engine
{
	void CameraInterface::Activate(Entity entity)
	{
		// Deactivate all other cameras
	}

	void CameraInterface::Deactivate(Entity entity)
	{
		// Deactivate camera.
	}

	CameraInterface::CameraInterface(ECS& refECS) : 
		m_refECS(refECS) {}

	Camera* CameraInterface::GetCamera(Entity entity)
	{
		return m_refECS.GetComponent<Camera>(entity);
	}

	void CameraInterface::SetPixelsPerUnit(Entity entity, const int pixelsPerUnit)
	{
		GetCamera(entity)->m_pixelsPerUnit = pixelsPerUnit;
		SetSize(entity);
	}

	const int CameraInterface::GetPixelsPerUnit(Entity entity)
	{
		return GetCamera(entity)->m_pixelsPerUnit;
	}

	void CameraInterface::SetSize(Entity entity)
	{
		Camera* ptrCamera = GetCamera(entity);
		ptrCamera->m_size
			= Vector2D<float>(Screen::VIEWPORT_SIZE.X / (ptrCamera->m_pixelsPerUnit * Screen::SCALE.X), 
				Screen::VIEWPORT_SIZE.Y / (ptrCamera->m_pixelsPerUnit * Screen::SCALE.Y));
	}

	const Vector2D<float> CameraInterface::GetSize(Entity entity)
	{
		return GetCamera(entity)->m_size;
	}

	void CameraInterface::SetOffsets(Entity entity, const Vector2D<float> offsets)
	{
		GetCamera(entity)->m_offset = offsets;
	}

	void CameraInterface::SetOffset(Entity entity, const Vector2D<float> offset)
	{
		GetCamera(entity)->m_offset = offset;
	}

	const Vector2D<float> CameraInterface::GetOffset(Entity entity)
	{
		return GetCamera(entity)->m_offset;
	}

	void CameraInterface::SetClampingOn(Entity entity, const bool clampingOn)
	{
		GetCamera(entity)->m_clampingOn = clampingOn;
	}

	const bool CameraInterface::GetClampingOn(Entity entity)
	{
		return GetCamera(entity)->m_clampingOn;
	}
}