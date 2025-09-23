#pragma once

#include "../../include/Camera/CameraSystem.h"
#include "../../include/Camera/CameraInterface.h"
#include "../../include/Rendering/Screen.h"
#include "../../include/Time.h"
#include "../../include/ECS/ECS.h"
#include "../../include/Components.h"

#include "../../include/Logging/Logger.h"

namespace Engine
{
    CameraSystem::CameraSystem(ECS& refECS) : m_refECS(refECS), m_currentViewportSizeInTiles(0.0f, 0.0f) {}

    void CameraSystem::Update()
    {
        for (auto& refCamera : m_refECS.GetHotComponents<Camera>())
        {
            CameraUpdater* ptrCameraUpdater = m_refECS.GetComponent<CameraUpdater>(refCamera.m_entity);
            if (ptrCameraUpdater)
                ptrCameraUpdater->Update(refCamera.m_entity);

            if (refCamera.m_clampingOn) Clamp(refCamera);

            // viewport size in tiles
            float viewportSizeInTilesX = Screen::VIEWPORT_SIZE.X / (refCamera.m_pixelsPerUnit * Screen::SCALE.X);
            float viewportSizeInTilesY = Screen::VIEWPORT_SIZE.Y / (refCamera.m_pixelsPerUnit * Screen::SCALE.Y);

			const bool viewPortSizeChanged = m_currentViewportSizeInTiles != Vector2D<float>(viewportSizeInTilesX, viewportSizeInTilesY);

            if (!viewPortSizeChanged) continue;

            // Calculate the frame and corresponding camera offset on screen.
            refCamera.m_screenOffset.X = refCamera.m_offset.X - refCamera.m_position.X * viewportSizeInTilesX;
            refCamera.m_screenOffset.Y = refCamera.m_offset.Y - refCamera.m_position.Y * viewportSizeInTilesY;

            refCamera.m_bottomRightCorner.X = refCamera.m_offset.X + refCamera.m_screenRatio.X * viewportSizeInTilesX;
            refCamera.m_bottomRightCorner.Y = refCamera.m_offset.Y + refCamera.m_screenRatio.Y * viewportSizeInTilesY;

            refCamera.m_frameDimensions.X = refCamera.m_bottomRightCorner.X - refCamera.m_offset.X;
            refCamera.m_frameDimensions.Y = refCamera.m_bottomRightCorner.Y - refCamera.m_offset.Y;

			m_currentViewportSizeInTiles = Vector2D<float>(viewportSizeInTilesX, viewportSizeInTilesY);
        }
    }

    void CameraSystem::Clamp(Camera& refCamera)
    {
        if (refCamera.m_offset.X < 0) { refCamera.m_offset.X = 0; }
        if (refCamera.m_offset.X + refCamera.m_size.X > refCamera.m_bounds.X) { refCamera.m_offset.X = refCamera.m_bounds.X - refCamera.m_size.X; }

        if (refCamera.m_offset.Y < 0) { refCamera.m_offset.Y = 0; }
        if (refCamera.m_offset.Y + refCamera.m_size.Y > refCamera.m_bounds.Y) { refCamera.m_offset.Y = refCamera.m_bounds.Y - refCamera.m_size.Y; }
    }

    void CameraSystem::Frame(Camera& refCamera, const Vector2D<int> mapBounds)
    {
        refCamera.m_bounds = mapBounds;
        refCamera.m_size
            = Vector2D<float>((Screen::VIEWPORT_SIZE.X * refCamera.m_screenRatio.X) / (refCamera.m_pixelsPerUnit * Screen::SCALE.X),
                (Screen::VIEWPORT_SIZE.Y * refCamera.m_screenRatio.Y) / (refCamera.m_pixelsPerUnit * Screen::SCALE.Y));

		// If the entity with the camera has a transform component, center the camera on the transform position
		if (m_refECS.HasComponent<Transform>(refCamera.m_entity))
		{
			Transform* ptrTransform = m_refECS.GetComponent<Transform>(refCamera.m_entity);
			refCamera.m_offset.X = ptrTransform->Position.X - (refCamera.m_size.X) / 2;
			refCamera.m_offset.Y = ptrTransform->Position.Y - (refCamera.m_size.Y) / 2;
		}

		Clamp(refCamera);
    }
}