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

	public:
		Camera* m_ptrCurrentCamera;
	};
}