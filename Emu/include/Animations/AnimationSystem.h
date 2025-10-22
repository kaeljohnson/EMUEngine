#pragma once

#include "../ECS/ECS.h"

namespace Engine
{
	class AnimationSystem
	{
	public:
		AnimationSystem(ECS& refECS);
		~AnimationSystem() = default;

		/*
			Update all animations in the system. Based on frame time.
			arg1: deltaTime: The time elapsed since the last update (in seconds).
		*/
		void Update();

	private:
		ECS& m_refECS;
	};
}