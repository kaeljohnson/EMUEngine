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
    CameraSystem::CameraSystem(ECS& refECS) : m_refECS(refECS) {}

    void CameraSystem::Update()
    {
        for (auto& camera : m_refECS.GetHotComponents<Camera>())
        {
            CameraUpdater* ptrCameraUpdater = m_refECS.GetComponent<CameraUpdater>(camera.m_entity);
            if (ptrCameraUpdater)
                ptrCameraUpdater->Update(camera.m_entity);

            if (camera.m_clampingOn) Clamp(camera);
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
            = Vector2D<float>(Screen::VIEWPORT_SIZE.X / (refCamera.m_pixelsPerUnit * Screen::SCALE.X),
                Screen::VIEWPORT_SIZE.Y / (refCamera.m_pixelsPerUnit * Screen::SCALE.Y));
    }
}