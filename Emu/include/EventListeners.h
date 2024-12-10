#pragma once

#include <variant>
#include <functional>
#include <cstddef>
#include <utility>

#include "ECS/Entity.h"

#include "Events/IOEvent.h"

#include "MathUtil.h"

namespace Engine
{
	

	// Begin contact event handling
	struct SingleEntityContactKey
	{
		SingleEntityContactKey(const Entity* entity)
			: m_key(entity->GetID()) {}

		bool operator==(const SingleEntityContactKey& other) const
		{
			return m_key == other.m_key;
		}

		size_t m_key; // The unique key
	};

	struct MultiEntityContactKey
	{
		MultiEntityContactKey(const Entity* entity1, const Entity* entity2)
			: m_key(GenerateKey(entity1->GetID(), entity2->GetID())) {}

		bool operator==(const MultiEntityContactKey& other) const
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

	struct ContactEvent
	{
		ContactEvent(Entity* ptrEntityA, Entity* ptrEntityB)
			: m_ptrEntityA(ptrEntityA), m_ptrEntityB(ptrEntityB) {}

		Entity* m_ptrEntityA;
		Entity* m_ptrEntityB;
	};

	struct BeginContact : public ContactEvent
	{
		BeginContact(Entity* ptrEntityA, Entity* ptrEntityB, Vector2D<float> normalVec)
			: m_normalVec(normalVec), ContactEvent(ptrEntityA, ptrEntityB) {}

		Vector2D<float> m_normalVec;
	};

	struct EndContact : public ContactEvent
	{
		EndContact(Entity* ptrEntityA, Entity* ptrEntityB)
			: ContactEvent(ptrEntityA, ptrEntityB) {}
	};

	struct BeginSensing : public ContactEvent
	{
		BeginSensing(Entity* ptrEntityA, Entity* ptrEntityB)
			: ContactEvent(ptrEntityA, ptrEntityB) {}
	};

	struct EndSensing : public ContactEvent
	{
		EndSensing(Entity* ptrEntityA, Entity* ptrEntityB)
			: ContactEvent(ptrEntityA, ptrEntityB) {}
	};

	struct ContactEventListener
	{
		ContactEventListener(Entity* ptrEntity,
			std::function<void(BeginContact)> onBeginContact,
			std::function<void(EndContact)> onEndContact)
			: m_onBeginContact(onBeginContact), m_onEndContact(onEndContact) {}

		std::function<void(BeginContact)> m_onBeginContact;
		std::function<void(EndContact)> m_onEndContact;
	};

	struct SensorEventListener        
	{
		SensorEventListener(Entity* ptrEntity,
			std::function<void(BeginSensing)> onBeginSensing,
			std::function<void(EndSensing)> onEndSensing)
			: m_onBeginSensing(onBeginSensing), m_onEndSensing(onEndSensing) {}

		std::function<void(BeginSensing)> m_onBeginSensing;
		std::function<void(EndSensing)> m_onEndSensing;
	};
}

namespace std
{
	template<>
	struct hash<Engine::SingleEntityContactKey>
	{
		std::size_t operator()(const Engine::SingleEntityContactKey& key) const
		{
			return key.m_key;
		}
	};

	template <>
	struct hash<Engine::MultiEntityContactKey>
	{
		std::size_t operator()(const Engine::MultiEntityContactKey& key) const
		{
			return key.m_key;
		}
	};
}