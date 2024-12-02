#pragma once

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

	struct SimpleContact : public Component
	{
		SimpleContact(Entity* ptrEntity) : Component(ptrEntity) {}

		bool m_contactAbove = false;
		bool m_contactBelow = false;
		bool m_contactRight = false;
		bool m_contactLeft = false;
	};
}