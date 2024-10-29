#pragma once

#include "../Core.h"

#include "Camera.h"

#include "../Transform.h"

//namespace Engine
//{
//	class TargetCamera : public Camera
//	{
//	public:
//		EMU_API TargetCamera(const size_t targetEntityID);
//		EMU_API void SetCameraTarget(const size_t targetEntityID);
//		EMU_API virtual void Update(const double interpolation);
//
//		~TargetCamera() = default;
//	
//	protected:
//		size_t m_cameraTargetEntityID;
//	};
//}