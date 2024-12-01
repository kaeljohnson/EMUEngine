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
	
	// Temp
	struct BeginContact
	{
		BeginContact(Entity* ptrEntity, ContactDirection direction)
			: m_ptrOtherEntity(ptrEntity), m_direction(direction) {}

		Entity* m_ptrOtherEntity;

		ContactDirection m_direction;
	};

	struct EndContact
	{
		EndContact(Entity* ptrEntity)
			: m_ptrOtherEntity(ptrEntity) {}

		Entity* m_ptrOtherEntity;
	};

	// May be a better way to do this... Via an event listener system?
	struct ContactEventListener : public Component
	{
		ContactEventListener(Entity* ptrEntity, 
			std::function<void(BeginContact)> onBeginContact,
			std::function<void(EndContact)> onEndContact) 
			: m_onBeginContact(onBeginContact), m_onEndContact(onEndContact), Component(ptrEntity) {}

		std::function<void(BeginContact)> m_onBeginContact;
		std::function<void(EndContact)> m_onEndContact;
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