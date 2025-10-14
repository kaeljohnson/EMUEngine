#pragma once

#include "../ECS/ECS.h"

namespace Engine
{
	class AnimationSystem
	{
	public:
		AnimationSystem(ECS& refECS);
		~AnimationSystem() = default;

		void Update(float deltaTime);

	private:
		ECS& m_refECS;
	};
}