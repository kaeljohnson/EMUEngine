#pragma once

#include "../Core.h"
#include "../ECS/Entity.h"
#include "Camera.h"

namespace Engine
{
	class CameraManager
	{
	public:
		EMU_API void SetCurrentCamera(Entity* ptrCameraEntity);

	public:
		CameraManager();
		~CameraManager() = default;

		CameraManager(const CameraManager&) = delete;
		CameraManager& operator=(const CameraManager&) = delete;
		CameraManager(CameraManager&&) = delete;
		CameraManager& operator=(CameraManager&&) = delete;

	public:
		Entity* m_ptrCurrentCameraEntity;
	};
}