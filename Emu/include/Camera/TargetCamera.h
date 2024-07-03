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

		EMU_API void SetTargetSmoothingFactor(const float smoothingFactor);
		EMU_API void SetRightTargetScreenBound(const float screenBound);
		EMU_API void SetLeftTargetScreenBound(const float screenBound);
		EMU_API void SetTopTargetScreenBound(const float screenBound);
		EMU_API void SetBottomTargetScreenBound(const float screenBound);

		~TargetCamera() = default;

	public:
		void Update(const double interpolation) override;
	private:
		SceneObject* ptrCameraTarget;

		bool m_smoothingOn;

		float m_smoothingFactor;
		float m_rightTargetScreenBound;
		float m_leftTargetScreenBound;
		float m_topTargetScreenBound;
		float m_bottomTargetScreenBound;
	};
}