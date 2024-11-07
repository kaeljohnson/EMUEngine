#pragma once

#include "../Core.h"

#include "Camera.h"
#include "../ECS/Entity.h"


namespace Engine
{
	class CameraManager
	{
	public:
		EMU_API void SetCurrentCamera(Entity* cameraEntity);

	public:
		CameraManager();
		~CameraManager() = default;

		CameraManager(const CameraManager&) = delete;
		CameraManager& operator=(const CameraManager&) = delete;
		CameraManager(CameraManager&&) = delete;
		CameraManager& operator=(CameraManager&&) = delete;

	public:
		Entity* m_currentCameraEntity;
	};
}