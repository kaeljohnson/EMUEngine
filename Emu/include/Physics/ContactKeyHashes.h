#pragma once

#include "ContactSystem.h"

namespace Engine
{
	/**
	* @brief Generates a unique key for a pair of entity IDs.
	 *
	 * @param id1 The first entity ID.
	 * @param id2 The second entity ID.
	 * @return A unique size_t key representing the pair of entity IDs.
	 */
	static size_t GenerateKey(size_t id1, size_t id2)
	{
		if (id1 > id2) std::swap(id1, id2); // Ensure consistent order

		// Combine the two IDs into a unique key
		return std::hash<size_t>()(id1) ^ (std::hash<size_t>()(id2) << 1);
	}

	/** 
	* @struct Contact key
	* 
	* @brief Contact key for contact event handling
	*/
	struct ContactKey
	{
		/**
		* @brief Constructor
		* 
		* @param key The unique key
		*/
		ContactKey(const size_t key) : m_key(key) {}

		/**
		* @brief Equality operator
		* 
		* @param other The other ContactKey to compare with
		* 
		* @return True if the keys are equal, false otherwise
		*/
		bool operator==(const ContactKey& other) const
		{
			return m_key == other.m_key;
		}

		size_t m_key; /// The unique key
	};

	/**
	* @struct Different contact key types for different contact events
	* 
	* @brief Different contact key types for different contact events
	* 
	* Each struct inherits from ContactKey and represents a specific type of contact event.
	*/
	struct SingleEntityBeginContactKey : public ContactKey
	{
		SingleEntityBeginContactKey(const Entity entity)
			: ContactKey(entity) {}
	};

	/**
	* @struct Different contact key types for different contact events
	* 
	* @brief Different contact key types for different contact events
	* 
	* Each struct inherits from ContactKey and represents a specific type of contact event.
	*/
	struct MultiEntityBeginContactKey : public ContactKey
	{
		MultiEntityBeginContactKey(const Entity entity1, const Entity entity2)
			: ContactKey(GenerateKey(entity1, entity2)) {}
	};

	/**
	* @struct Different contact key types for different contact events
	* 
	* @brief Different contact key types for different contact events
	* 
	* Each struct inherits from ContactKey and represents a specific type of contact event.
	*/
	struct SingleEntityEndContactKey : public ContactKey
	{
		SingleEntityEndContactKey(const Entity entity)
			: ContactKey(entity) {}
	};

	/**
	* @struct Different contact key types for different contact events
	* 
	* @brief Different contact key types for different contact events
	* 
	* Each struct inherits from ContactKey and represents a specific type of contact event.
	*/
	struct MultiEntityEndContactKey : public ContactKey
	{
		MultiEntityEndContactKey(const Entity entity1, const Entity entity2)
			: ContactKey(GenerateKey(entity1, entity2)) {}
	};
}
namespace std
{
	/**
	* @brief Specializations of std::hash for contact key types
	*
	* These specializations allow the contact key types to be used as keys in unordered containers.
	*/
	template<>
	struct hash<Engine::SingleEntityBeginContactKey>
	{
		std::size_t operator()(const Engine::SingleEntityBeginContactKey& key) const
		{
			return key.m_key;
		}
	};

	/**
	* @brief Specializations of std::hash for contact key types\
	* 
	* These specializations allow the contact key types to be used as keys in unordered containers.
	*/
	template<>
	struct hash<Engine::MultiEntityBeginContactKey>
	{
		std::size_t operator()(const Engine::MultiEntityBeginContactKey& key) const
		{
			return key.m_key;
		}
	};

	/**
	* @brief Specializations of std::hash for contact key types
	* 
	* These specializations allow the contact key types to be used as keys in unordered containers.
	*/
	template<>
	struct hash<Engine::SingleEntityEndContactKey>
	{
		std::size_t operator()(const Engine::SingleEntityEndContactKey& key) const
		{
			return key.m_key;
		}
	};

	/**
	* @brief Specializations of std::hash for contact key types
	* 
	* These specializations allow the contact key types to be used as keys in unordered containers.
	*/
	template<>
	struct hash<Engine::MultiEntityEndContactKey>
	{
		std::size_t operator()(const Engine::MultiEntityEndContactKey& key) const
		{
			return key.m_key;
		}
	};
}