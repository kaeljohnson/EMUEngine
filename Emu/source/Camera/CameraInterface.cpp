#pragma once

#include "../../include/Camera/CameraInterface.h"
#include "../../Public/Screen.h"
#include "../../include/Components.h"
#include "../../Public/Logger.h"

namespace Engine
{
	CameraInterface::CameraInterface(ECS& refECS) : 
		m_refECS(refECS) {}

	void CameraInterface::SetPixelsPerUnit(Entity entity, const int pixelsPerUnit)
	{
		Camera* ptrCamera = m_refECS.GetComponent<Camera>(entity);

		ptrCamera->m_pixelsPerUnit = pixelsPerUnit;

		ptrCamera->m_size
			= Math2D::Point2D<float>(Screen::WINDOW_SIZE.X * ptrCamera->m_viewportSizeInPercentageOfScreen.X / (ptrCamera->m_pixelsPerUnit * Screen::SCALE),
				Screen::WINDOW_SIZE.Y * ptrCamera->m_viewportSizeInPercentageOfScreen.Y / (ptrCamera->m_pixelsPerUnit * Screen::SCALE));
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

	const Math2D::Point2D<int> CameraInterface::GetScreenPosition(Entity entity)
	{
		Transform* ptrTransform = m_refECS.GetComponent<Transform>(entity);
		return ptrTransform->m_positionOnScreen;
	}

	void CameraInterface::SetClampingOn(Entity entity, const bool clampingOn)
	{
		m_refECS.GetComponent<Camera>(entity)->m_clampingOn = clampingOn;
	}

	const bool CameraInterface::GetClampingOn(Entity entity)
	{
		return m_refECS.GetComponent<Camera>(entity)->m_clampingOn;
	}

	void CameraInterface::SetSize(const Math2D::Point2D<int> size)
	{
		std::vector<Camera>& activeCameras = m_refECS.GetHotComponents<Camera>();
		if (activeCameras.size() == 0)
		{
			ENGINE_ERROR("No active cameras in the scene. Cannot set camera size.");
			std::exit(1);
		}
		for (auto& refCamera : activeCameras)
		{
			refCamera.m_size = Math2D::Point2D<float>((Screen::WINDOW_SIZE.X * refCamera.m_viewportSizeInPercentageOfScreen.X) / (refCamera.m_pixelsPerUnit * Screen::SCALE),
				(Screen::WINDOW_SIZE.Y * refCamera.m_viewportSizeInPercentageOfScreen.Y) / (refCamera.m_pixelsPerUnit * Screen::SCALE));
		}
	}

	const bool CameraInterface::InFrame(Entity entity)
	{
		Transform* ptrTransform = m_refECS.GetComponent<Transform>(entity);
		if (!ptrTransform)
			return false;

		std::vector<Camera>& activeCameras = m_refECS.GetHotComponents<Camera>();
		for (auto& refCamera : activeCameras)
		{
			const bool inHorizontalBounds = ptrTransform->m_position.X + refCamera.m_size.X >= refCamera.m_offset.X &&
				ptrTransform->m_position.X <= refCamera.m_offset.X + refCamera.m_size.X;
			const bool inVerticalBounds = ptrTransform->m_position.Y + refCamera.m_size.Y >= refCamera.m_offset.Y &&
				ptrTransform->m_position.Y <= refCamera.m_offset.Y + refCamera.m_size.Y;
			if (inHorizontalBounds && inVerticalBounds)
				return true;
		}
		return false;
	}
}