#pragma once

#include "../../include/Animations/AnimationSystem.h"
#include "../../include/ECS/ECS.h"
#include "../../include/Components.h"

namespace Engine
{
	AnimationSystem::AnimationSystem(ECS& refECS) : m_refECS(refECS)
	{
		// Constructor implementation
	}

	void AnimationSystem::Update(float deltaTime)
	{
		// Update logic for animations
		// This should iterate through animations not sprites...
		for (auto& sprite : m_refECS.GetHotComponents<Sprite>())
		{
			if (sprite.m_animations.empty() || sprite.m_currentAnimation == "None")
			{
				ENGINE_CRITICAL_D("No animations found for sprite with entity ID: " + std::to_string(sprite.m_entity));
				continue; // No animations to update or invalid animation index
			}

			Animation& currentAnimation = sprite.m_animations[sprite.m_currentAnimation];

			currentAnimation.m_frameTime++;

			if (currentAnimation.m_frameTime < currentAnimation.m_frameDuration)
			{
				// Not enough time has passed to switch frames
				continue;
			}

			currentAnimation.m_frameTime = 0; // Reset frame time

			sprite.m_frameCounter++;

			// ENGINE_CRITICAL_D("Number of Frames: " + std::to_string(currentAnimation.m_numFrames));
			// ENGINE_CRITICAL_D("   Frame Counter: " + std::to_string(sprite.m_frameCounter));
			// ENGINE_CRITICAL_D("   Current Frame: " + std::to_string(sprite.m_currentFrame));

			if (sprite.m_frameCounter >= currentAnimation.m_numFrames && currentAnimation.m_loop)
			{
				// Start the animation from the beginning
				sprite.m_frameCounter = 0;
				sprite.m_currentFrame = currentAnimation.m_frames[0];
			}
			else if (sprite.m_frameCounter >= currentAnimation.m_numFrames && !currentAnimation.m_loop)
			{
				// Stop the animation at the last frame
				sprite.m_frameCounter = currentAnimation.m_numFrames - 1;
				sprite.m_currentFrame = currentAnimation.m_frames.back();
			}
			else
			{
				// Continue to the next frame in the animation
				sprite.m_currentFrame = currentAnimation.m_frames[sprite.m_frameCounter];
			}
		}
	}
}
