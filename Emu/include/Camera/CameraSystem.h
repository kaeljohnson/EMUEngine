#pragma once

#include "../Components.h"
#include "../ECS/ECS.h"

namespace Engine
{
	// Any camera component functions that have to do with updating the camera during processing should be in this class.
	// The client will not have access to this class.
	class CameraSystem
	{
	public:
		CameraSystem(ECS& refECS);
		~CameraSystem() = default;

		void Clamp(Camera& refCamera);

		void Update();

	private:
		ECS& m_refECS;
	};
}