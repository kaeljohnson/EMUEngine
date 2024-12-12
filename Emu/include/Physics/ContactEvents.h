#pragma once

#include <functional>
#include <cstddef>
#include <utility>

#include "../ECS/Entity.h"

#include "../MathUtil.h"

namespace Engine
{
	static size_t GenerateKey(size_t id1, size_t id2)
	{
		if (id1 > id2) std::swap(id1, id2); // Ensure consistent order

		// Combine the two IDs into a unique key
		return std::hash<size_t>()(id1) ^ (std::hash<size_t>()(id2) << 1);
	}

	// Contact key for contact event handling
	struct ContactKey
	{
		ContactKey(const size_t key) : m_key(key) {}

		bool operator==(const ContactKey& other) const
		{
			return m_key == other.m_key;
		}

		size_t m_key; // The unique key
	};

	struct SingleEntityBeginContactKey : public ContactKey
	{
		SingleEntityBeginContactKey(const Entity* entity) 
			: ContactKey(entity->GetID()) {}
	};

	struct MultiEntityBeginContactKey : public ContactKey
	{
		MultiEntityBeginContactKey(const Entity* entity1, const Entity* entity2)
			: ContactKey(GenerateKey(entity1->GetID(), entity2->GetID())) {}
	};

	struct SingleEntityEndContactKey : public ContactKey
	{
		SingleEntityEndContactKey(const Entity* entity)
			: ContactKey(entity->GetID()) {}
	};

	struct MultiEntityEndContactKey : public ContactKey
	{
		MultiEntityEndContactKey(const Entity* entity1, const Entity* entity2)
			: ContactKey(GenerateKey(entity1->GetID(), entity2->GetID())) {}
	};

	// Event wrapper objects
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

	struct ContactListener
	{
		ContactListener(size_t key) : m_key(key) {}

		virtual ~ContactListener() = default;

		virtual void OnContactBegin(const ContactEvent event) {};
		virtual void OnContactEnd(const ContactEvent event) {};

		size_t GetKey() const { return m_key; }

	private:
		size_t m_key;
	};

	struct SingleEntityContactListener : public ContactListener
	{
		SingleEntityContactListener(Entity* entity)
			: ContactListener(entity->GetID()) {}
	};

	struct MultiEntityContactListener : public ContactListener
	{
		MultiEntityContactListener(Entity* entity1, Entity* entity2)
			: ContactListener(GenerateKey(entity1->GetID(), entity2->GetID())) {}
	};

	struct SingleEntitySensorListener : public ContactListener
	{
		SingleEntitySensorListener(Entity* entity)
			: ContactListener(entity->GetID()) {}
	};

	struct MultiEntitySensorListener : public ContactListener
	{
		MultiEntitySensorListener(Entity* entity1, Entity* entity2)
			: ContactListener(GenerateKey(entity1->GetID(), entity2->GetID())) {}
	};
}

namespace std
{
	template<>
	struct hash<Engine::SingleEntityBeginContactKey>
	{
		std::size_t operator()(const Engine::SingleEntityBeginContactKey& key) const
		{
			return key.m_key;
		}
	};

	template<>
	struct hash<Engine::MultiEntityBeginContactKey>
	{
		std::size_t operator()(const Engine::MultiEntityBeginContactKey& key) const
		{
			return key.m_key;
		}
	};

	template<>
	struct hash<Engine::SingleEntityEndContactKey>
	{
		std::size_t operator()(const Engine::SingleEntityEndContactKey& key) const
		{
			return key.m_key;
		}
	};

	template<>
	struct hash<Engine::MultiEntityEndContactKey>
	{
		std::size_t operator()(const Engine::MultiEntityEndContactKey& key) const
		{
			return key.m_key;
		}
	};
}