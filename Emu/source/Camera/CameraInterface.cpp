#pragma once

#include "../../include/Camera/CameraInterface.h"
#include "../../include/Rendering/Screen.h"
#include "../../include/Components.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	void CameraInterface::ChangeCamera(Entity entity)
	{
		// Might support multiple active cameras in the future but for now 
		// deactivate all other cameras and activate the new one.

		if (!m_refECS.HasComponent<Camera>(entity)) return;

		// Deactivate all other cameras
		std::vector<Camera>& activeCameras = m_refECS.GetHotComponents<Camera>();

		if (activeCameras.size() > 1)
		{
			ENGINE_CRITICAL_D("Two or more active cameras should be impossible!");
			std::runtime_error("Two or more active cameras should be impossible!");
			return;
		}
	
		Entity activeCameraEntity = activeCameras[0].m_entity;

		m_refECS.DeactivateComponent<Camera>(activeCameraEntity);
		m_refECS.DeactivateComponent<CameraUpdater>(activeCameraEntity);
		m_refECS.ActivateComponent<Camera>(entity);
		m_refECS.ActivateComponent<CameraUpdater>(entity);
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