#pragma once

#include "../../include/Animations/AnimationSystem.h"
#include "../../include/ECS/ECS.h"
#include "../../include/Components.h"

namespace Engine
{
	AnimationSystem::AnimationSystem(ECS& refECS) : m_refECS(refECS) {}

	void AnimationSystem::Update()
	{
		// auto now = std::chrono::high_resolution_clock::now();

		// Update logic for animations
		for (auto& animations : m_refECS.GetHotComponents<Animations>())
		{
			if (animations.m_animations.empty() || animations.m_currentAnimation == "None")
			{
				ENGINE_CRITICAL_D("No animations found for sprite with entity ID {}", animations.m_entity);
				continue; // No animations to update or invalid animation index
			}

			Animation& currentAnimation = animations.m_animations[animations.m_currentAnimation];

			currentAnimation.m_frameTime++;

			if (currentAnimation.m_frameTime < currentAnimation.m_frameDuration)
			{
				// Not enough time has passed to switch frames
				continue;
			}

			currentAnimation.m_frameTime = 0; // Reset frame time

			currentAnimation.m_frameCounter++;

			if (currentAnimation.m_frameCounter >= currentAnimation.m_numFrames && currentAnimation.m_loop)
			{
				// Start the animation from the beginning
				currentAnimation.m_frameCounter = 0;
				currentAnimation.m_currentFrame = currentAnimation.m_frames[0];
			}
			else if (currentAnimation.m_frameCounter >= currentAnimation.m_numFrames && !currentAnimation.m_loop)
			{
				// Stop the animation at the last frame
				currentAnimation.m_frameCounter = currentAnimation.m_numFrames - 1;
				currentAnimation.m_currentFrame = currentAnimation.m_frames.back();
			}
			else
			{
				// Continue to the next frame in the animation
				currentAnimation.m_currentFrame = currentAnimation.m_frames[currentAnimation.m_frameCounter];
			}
		}

		// auto end = std::chrono::high_resolution_clock::now();
		// std::chrono::duration<double, std::milli> elapsed = end - now;
		// ENGINE_TRACE_D("AnimationSystem Update took " + std::to_string(elapsed.count()) + " ms");
	}
}
