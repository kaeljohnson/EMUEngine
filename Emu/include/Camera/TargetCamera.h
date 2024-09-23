#pragma once

#include "../Core.h"

#include "Camera.h"

#include "../Transform.h"

namespace Engine
{
	class TargetCamera : public Camera
	{
	public:
		EMU_API TargetCamera();
		EMU_API void SetCameraTarget(Transform* ptrTarget);
		EMU_API virtual void Update(const double interpolation);

		~TargetCamera() = default;
	
	protected:
		Transform* ptrCameraTarget;
	};
}