#pragma once

#include "../Includes.h"
#include "../ECS/ECS.h"

namespace Engine
{ 
	/**
	* @class AnimationInterface
	* 
	* @brief An interface for client to change animations.
	*/
	class AnimationInterface
	{
	public:
		AnimationInterface(ECS& refECS);
		~AnimationInterface() = default;

		void PlayAnimation(const Entity entity, const size_t animationId);

	private:
		ECS& m_refECS;	/// Ref to the ECS instance.
	};
}