#pragma once

#include "../ECS/ECS.h"

namespace Engine
{
	/**
	 * @brief Animation system.
	 *
	 * Updates the animation component of each entity based on frame time.
	 */
	class AnimationSystem
	{
	public:
		/**
		 * @brief Constructs the animation system.
		 * @param refECS Reference to the ECS instance.
		 */
		AnimationSystem(ECS& refECS);

		~AnimationSystem() = default;

		/**
		 * @brief Updates all animations in the system.
		 *
		 * Uses frame delta time to advance animation state.
		 */
		void Update();

	private:
		ECS& m_refECS;
	};
}
