#pragma once

#include "../../include/Animations/AnimationInterface.h"
#include "../../include/Components.h"

namespace Engine
{
	AnimationInterface::AnimationInterface(ECS& refECS)
		: m_refECS(refECS) {}

	void AnimationInterface::PlayAnimation(const Entity entity, const size_t animationId)
	{
		Animations* ptrAnimations = m_refECS.GetComponent<Animations>(entity);
		if (!ptrAnimations)
		{
			ENGINE_CRITICAL("Entity ID {} does not have an Animations component.", entity);
			return;
		}

		if (ptrAnimations->m_animations.find(animationId) == ptrAnimations->m_animations.end())
		{
			ENGINE_CRITICAL("Animation '{}' not found for entity ID {}.", animationId, entity);
			return;
		}

		Animation& currentAnimation = ptrAnimations->m_animations[ptrAnimations->m_currentAnimation];

		currentAnimation.m_frameCounter = 0;
		currentAnimation.m_currentFrame = currentAnimation.m_frames[0];

		ptrAnimations->m_currentAnimation = animationId;
	}
}