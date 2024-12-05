#pragma once

#include <functional>

#include "ECS/Component.h"
#include "ECS/Entity.h"

#include "Events/IOEvent.h"

#include "MathUtil.h"

namespace Engine
{
	// Temp
	struct BeginContact
	{
		BeginContact(Entity* ptrEntity, Vector2D<float> normalVec)
			: m_ptrOtherEntity(ptrEntity) {}

		Entity* m_ptrOtherEntity;
	};

	struct EndContact
	{
		EndContact(Entity* ptrEntity)
			: m_ptrOtherEntity(ptrEntity) {}

		Entity* m_ptrOtherEntity;
	};

	struct BeginSensing
	{
		BeginSensing(Entity* ptrEntity)
			: m_ptrOtherEntity(ptrEntity) {}

		Entity* m_ptrOtherEntity;
	};

	struct EndSensing
	{
		EndSensing(Entity* ptrEntity)
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

	struct SensorEventListener : public Component
	{
		SensorEventListener(Entity* ptrEntity,
			std::function<void(BeginSensing)> onBeginSensing,
			std::function<void(EndSensing)> onEndSensing)
			: m_onBeginSensing(onBeginSensing), m_onEndSensing(onEndSensing), Component(ptrEntity) {}

		std::function<void(BeginSensing)> m_onBeginSensing;
		std::function<void(EndSensing)> m_onEndSensing;
	};
}