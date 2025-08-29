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
		void AddAnimation(Entity entity, const std::string& animationName);
		void RemoveAnimation(Entity entity, const std::string& animationName);
		void PlayAnimation(Entity entity, const std::string& animationName);
		void StopAnimation(Entity entity);

	private:
		ECS& m_refECS;
	};
}