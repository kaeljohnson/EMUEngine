#pragma once

#include "../../include/Camera/CameraSystem.h"
#include "../../include/Camera/CameraInterface.h"
#include "../../include/Rendering/Screen.h"
#include "../../include/Time.h"
#include "../../include/ECS/ECS.h"
#include "../../include/Components.h"

namespace Engine
{
    CameraSystem::CameraSystem(ECS& refECS) : m_refECS(refECS) {}

    void CameraSystem::Update()
    {
        for (auto& camera : m_refECS.GetComponentManager<Camera>())
        {
            if (camera.IsActive())
            {
                // CAMERA UPDATES
                Engine::Transform* ptrCameraTarget = m_refECS.GetComponentManager<Transform>().GetComponent(camera.GetEntity());
                // Engine::Camera* ptrCamera = Engine::EMU::GetInstance()->IECS().GetComponentManager<Engine::Camera>().GetComponent(m_ptrEntity);

                // CLIENT_INFO_D("Interpolation Factor: " + std::to_string(Engine::Time::GetInterpolationFactor()));

                const float interpFactor = Engine::Time::GetInterpolationFactor();

                float targetX = Engine::Lerp(ptrCameraTarget->PrevPosition.X, ptrCameraTarget->Position.X, interpFactor);
                float targetY = Engine::Lerp(ptrCameraTarget->PrevPosition.Y, ptrCameraTarget->Position.Y, interpFactor);

                camera.m_offset.X = targetX - (camera.m_size.X / 2.0f);
                camera.m_offset.Y = targetY - (camera.m_size.Y / 2.0f);

                // CLIENT_INFO_D("Camera Offset: " + std::to_string(camera.m_offset.X) + ", " + std::to_string(ptrCamera->m_offset.Y));

                if (camera.m_clampingOn) Clamp(camera);
            }
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