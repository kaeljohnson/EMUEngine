#pragma once

#include "../../include/Camera/CameraInterface.h"
#include "../../include/Rendering/Screen.h"
#include "../../include/Components.h"

namespace Engine
{
	void CameraInterface::Activate(Entity entity)
	{
		// Deactivate all other cameras
		if (m_activeCameraEntity != -1)
		{
			m_refECS.Deactivate(m_activeCameraEntity);
		}
		m_refECS.Activate(entity);
		m_activeCameraEntity = entity;
	}

	CameraInterface::CameraInterface(ECS& refECS) : 
		m_refECS(refECS), m_activeCameraEntity(-1) {}

	Camera* CameraInterface::GetCamera(Entity entity)
	{
		return m_refECS.GetComponent<Camera>(entity);
	}

	void CameraInterface::SetPixelsPerUnit(Entity entity, const int pixelsPerUnit)
	{
		GetCamera(entity)->m_pixelsPerUnit = pixelsPerUnit;
		SetSize(entity);
	}

	void CameraInterface::SetPixelsPerUnit(Camera& camera, const int pixelsPerUnit)
	{
		camera.m_pixelsPerUnit = pixelsPerUnit;
		SetSize(camera);
	}

	const int CameraInterface::GetPixelsPerUnit(Entity entity)
	{
		return GetCamera(entity)->m_pixelsPerUnit;
	}

	const int CameraInterface::GetPixelsPerUnit(Camera& camera)
	{
		return camera.m_pixelsPerUnit;
	}

	void CameraInterface::SetSize(Entity entity)
	{
		Camera* ptrCamera = GetCamera(entity);
		ptrCamera->m_size
			= Vector2D<float>(Screen::VIEWPORT_SIZE.X / (ptrCamera->m_pixelsPerUnit * Screen::SCALE.X), 
				Screen::VIEWPORT_SIZE.Y / (ptrCamera->m_pixelsPerUnit * Screen::SCALE.Y));
	}

	void CameraInterface::SetSize(Camera& camera)
	{
		camera.m_size
			= Vector2D<float>(Screen::VIEWPORT_SIZE.X / (camera.m_pixelsPerUnit * Screen::SCALE.X),
				Screen::VIEWPORT_SIZE.Y / (camera.m_pixelsPerUnit * Screen::SCALE.Y));
	}

	const Vector2D<float> CameraInterface::GetSize(Entity entity)
	{
		return GetCamera(entity)->m_size;
	}

	const Vector2D<float> CameraInterface::GetSize(Camera& camera)
	{
		return camera.m_size;
	}

	void CameraInterface::SetOffsets(Entity entity, const Vector2D<float> offsets)
	{
		GetCamera(entity)->m_offset = offsets;
	}

	void CameraInterface::SetOffsets(Camera& camera, const Vector2D<float> offsets)
	{
		camera.m_offset = offsets;
	}

	void CameraInterface::SetOffset(Entity entity, const Vector2D<float> offset)
	{
		GetCamera(entity)->m_offset = offset;
	}

	void CameraInterface::SetOffset(Camera& camera, const Vector2D<float> offset)
	{
		camera.m_offset = offset;
	}

	const Vector2D<float> CameraInterface::GetOffset(Entity entity)
	{
		return GetCamera(entity)->m_offset;
	}

	const Vector2D<float> CameraInterface::GetOffset(Camera& camera)
	{
		return camera.m_offset;
	}

	void CameraInterface::SetClampingOn(Entity entity, const bool clampingOn)
	{
		GetCamera(entity)->m_clampingOn = clampingOn;
	}

	void CameraInterface::SetClampingOn(Camera& camera, const bool clampingOn)
	{
		camera.m_clampingOn = clampingOn;
	}

	const bool CameraInterface::GetClampingOn(Entity entity)
	{
		return GetCamera(entity)->m_clampingOn;
	}

	const bool CameraInterface::GetClampingOn(Camera& camera)
	{
		return camera.m_clampingOn;
	}
}