#pragma once

#include <functional>

#include "../ECS/Entity.h"

namespace Engine
{

	// Temp
	struct BeginContact
	{
		BeginContact(Entity* ptrEntity)
			: m_ptrOtherEntity(ptrEntity) {}

		Entity* m_ptrOtherEntity;
	};

	struct EndContact
	{
		EndContact(Entity* ptrEntity)
			: m_ptrOtherEntity(ptrEntity) {}

		Entity* m_ptrOtherEntity;
	};

	// May be a better way to do this... Via an event listener system?
	struct ContactEventListener
	{
		ContactEventListener(Entity* ptrEntity,
			std::function<void(BeginContact)> onBeginContact,
			std::function<void(EndContact)> onEndContact)
			: m_onBeginContact(onBeginContact), m_onEndContact(onEndContact) {}

		std::function<void(BeginContact)> m_onBeginContact;
		std::function<void(EndContact)> m_onEndContact;
	};
}