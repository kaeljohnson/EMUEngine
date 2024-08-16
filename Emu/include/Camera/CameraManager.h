#pragma once

#include "../Core.h"

#include "Camera.h"


namespace Engine
{
	class CameraManager
	{
	public:
		EMU_API void SetCurrentCamera(Camera* ptrCamera);

	public:
		CameraManager();
		~CameraManager() = default;

		CameraManager(const CameraManager&) = delete;
		CameraManager& operator=(const CameraManager&) = delete;
		CameraManager(CameraManager&&) = delete;
		CameraManager& operator=(CameraManager&&) = delete;

	public:
		Camera* m_ptrCurrentCamera;
	};
}