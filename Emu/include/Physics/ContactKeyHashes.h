#pragma once

#include "ContactSystem.h"

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