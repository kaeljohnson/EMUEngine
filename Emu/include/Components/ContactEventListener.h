#pragma once

#include <functional>

#include "../ECS/Entity.h"
#include "../ECS/Component.h"

#include "../MathUtil.h"

namespace Engine
{
	enum ContactDirection
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	// May be a better way to do this... Via an event listener system?
	struct ContactEventListener : public Component
	{
		std::function<void(Entity* ptrEntity, const ContactDirection direction)> OnBeginContact;
		std::function<void(Entity* ptrEntity)> OnEndContact;
	};
}