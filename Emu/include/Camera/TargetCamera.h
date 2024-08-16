#pragma once

#include "../Core.h"

#include "Camera.h"

#include "../Scenes/SceneObject.h"

namespace Engine
{
	class TargetCamera : public Camera
	{
	public:
		EMU_API TargetCamera();
		EMU_API void SetCameraTarget(SceneObject* ptrTarget);
		EMU_API virtual void Update(const double interpolation);

		~TargetCamera() = default;
	
	protected:
		SceneObject* ptrCameraTarget;
	};
}