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

		/**
		* @brief Plays the specified animation on the given entity.
		* 
		* @param entity The entity to play the animation on.
		* @param animationId The ID of the animation to play.
		*/
		void PlayAnimation(const Entity entity, const size_t animationId);

	private:
		ECS& m_refECS;	/// Ref to the ECS instance.
	};
}