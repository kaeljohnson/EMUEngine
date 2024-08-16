#pragma once

#include "../Core.h"
#include "../Scenes/SceneObject.h"
#include "Camera.h"

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