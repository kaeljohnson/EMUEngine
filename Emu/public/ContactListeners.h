#pragma once
#include "Contacts.h"
#include "ContactKeyHashes.h"

namespace Engine
{
	using Entity = size_t;

	/**
	* @struct ContactListener
	*
	* @brief Base class for contact listeners.
	*/
	struct ContactListener
	{
		/**
		* @brief Constructor
		*
		* @param key The unique key for the contact listener
		*/
		ContactListener(size_t key) : m_key(key) {}

		virtual ~ContactListener() = default;

		/**
		* @brief Called when a contact begins.
		*/
		virtual void OnContactBegin(const Contact event) {};

		/**
		* @brief Called when a contact ends.
		*/
		virtual void OnContactEnd(const Contact event) {};

		/**
		* @brief Gets the unique key for the contact listener.
		*/
		size_t GetKey() const { return m_key; }

	private:
		const size_t m_key; /// The unique key for the contact listener
	};

	/**
	* @struct SingleEntityContactListener
	*
	* @brief Contact listener for single entity contacts.
	*/
	struct SingleEntityContactListener : public ContactListener
	{
		/**
		* @brief Constructor
		*
		* @param entity The entity to listen for contacts on
		*/
		SingleEntityContactListener(Entity entity)
			: ContactListener(entity) {
		}
	};

	/**
	* @struct MultiEntityContactListener
	*
	* @brief Contact listener for multi-entity contacts.
	*/
	struct MultiEntityContactListener : public ContactListener
	{
		/**
		* @brief Constructor
		*
		* @param entity1 The first entity to listen for contacts on
		* @param entity2 The second entity to listen for contacts on
		*/
		MultiEntityContactListener(Entity entity1, Entity entity2)
			: ContactListener(GenerateKey(entity1, entity2)) {
		}
	};

	/**
	* @struct SingleEntitySensorListener
	*
	* @brief Sensor listener for single entity sensors.
	*
	*/
	struct SingleEntitySensorListener : public ContactListener
	{
		SingleEntitySensorListener(Entity entity)
			: ContactListener(entity) {
		}
	};

	/**
	* @struct MultiEntitySensorListener
	*
	* @brief Sensor listener for multi-entity sensors.
	*/
	struct MultiEntitySensorListener : public ContactListener
	{
		/**
		* @brief Constructor
		*
		* @param entity1 The first entity to listen for sensors on
		* @param entity2 The second entity to listen for sensors on
		*/
		MultiEntitySensorListener(Entity entity1, Entity entity2)
			: ContactListener(GenerateKey(entity1, entity2)) {
		}
	};

}