#pragma once

#include <functional>
#include <cstddef>
#include <utility>

#include "ECS/Component.h"
#include "ECS/Entity.h"

#include "Events/IOEvent.h"

#include "MathUtil.h"

namespace Engine
{
	struct ContactKey
	{
		ContactKey(const Entity* entity1, const Entity* entity2)
			: m_key(GenerateKey(entity1->GetID(), entity2->GetID())) {}

		bool operator==(const ContactKey& other) const
		{
			return m_key == other.m_key;
		}

		size_t m_key; // The unique key

	private:
		static size_t GenerateKey(size_t id1, size_t id2)
		{
			if (id1 > id2) std::swap(id1, id2); // Ensure consistent order

			// Combine the two IDs into a unique key
			return std::hash<size_t>()(id1) ^ (std::hash<size_t>()(id2) << 1);
		}
	};

	// Temp
	struct BeginContact
	{
		BeginContact(Entity* ptrEntityA, Entity* ptrEntityB, Vector2D<float> normalVec)
			: m_ptrEntityA(ptrEntityA), m_ptrEntityB(ptrEntityB) {}

		Entity* m_ptrEntityA;
		Entity* m_ptrEntityB;
		Vector2D<float> m_normalVec;
	};

	struct EndContact
	{
		EndContact(Entity* ptrEntityA, Entity* ptrEntityB)
			: m_ptrEntityA(ptrEntityA), m_ptrEntityB(ptrEntityB) {}

		Entity* m_ptrEntityA;
		Entity* m_ptrEntityB;
	};

	struct BeginSensing
	{
		BeginSensing(Entity* ptrEntityA, Entity* ptrEntityB)
			: m_ptrEntityA(ptrEntityA), m_ptrEntityB(ptrEntityB) {}

		Entity* m_ptrEntityA;
		Entity* m_ptrEntityB;
	};

	struct EndSensing
	{
		EndSensing(Entity* ptrEntityA, Entity* ptrEntityB)
			: m_ptrEntityA(ptrEntityA), m_ptrEntityB(ptrEntityB) {}

		Entity* m_ptrEntityA;
		Entity* m_ptrEntityB;
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

namespace std
{
	template <>
	struct hash<Engine::ContactKey>
	{
		std::size_t operator()(const Engine::ContactKey& key) const
		{
			return key.m_key;
		}
	};
}