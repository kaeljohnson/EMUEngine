#pragma once

#include "../ECS/ECS.h"
#include "../Includes.h"
#include "../Components.h"
#include "../MathUtil.h"
#include "../TileMap/TileMap.h"

#include "ContactKeyHashes.h"

namespace Engine
{
	/**
	* @struct Contact
	* 
	* @brief Represents a contact event between two entities.
	* 
	* This struct serves as a base for specific contact events such as
	*/
	struct Contact
	{
		/**
		* @brief Constructor
		* 
		* @param entityA The first entity involved in the contact
		* @param entityB The second entity involved in the contact
		*/
		Contact(Entity entityA, Entity entityB)
			: m_ptrEntityA(entityA), m_ptrEntityB(entityB) {}

		Entity m_ptrEntityA; /// The first entity involved in the contact
		Entity m_ptrEntityB; /// The second entity involved in the contact
	};

	/**
	* @struct BeginContact
	* 
	* @brief Represents the beginning of a contact event between two entities.
	*/
	struct BeginContact : public Contact
	{
		/**
		* @brief Constructor
		* 
		* @param entityA The first entity involved in the contact
		* @param entityB The second entity involved in the contact
		* @param normalVec The normal vector of the contact
		*/
		BeginContact(Entity entityA, Entity entityB, Math2D::Point2D<float> normalVec)
			: m_normalVec(normalVec), Contact(entityA, entityB) {}

		Math2D::Point2D<float> m_normalVec; /// The normal vector of the contact
	};

	/**
	* @struct EndContact
	* 
	* @brief Represents the end of a contact event between two entities.
	*/
	struct EndContact : public Contact
	{
		/**
		* @brief Constructor
		* 
		* @param entityA The first entity involved in the contact
		* @param entityB The second entity involved in the contact
		*/
		EndContact(Entity entityA, Entity entityB)
			: Contact(entityA, entityB) {}
	};

	/**
	* @struct BeginSensing
	* 
	* @brief Represents the beginning of a sensing event between two entities.
	*/
	struct BeginSensing : public Contact
	{
		/**
		* @brief Constructor
		* 
		* @param entityA The first entity involved in the sensing
		* @param entityB The second entity involved in the sensing
		*/
		BeginSensing(Entity entityA, Entity entityB)
			: Contact(entityA, entityB) {}
	};

	/**
	* @struct EndSensing
	* 
	* @brief Represents the end of a sensing event between two entities.
	*/
	struct EndSensing : public Contact
	{
		/**
		* @brief Constructor
		* 
		* @param entityA The first entity involved in the sensing
		* @param entityB The second entity involved in the sensing
		*/
		EndSensing(Entity entityA, Entity entityB)
			: Contact(entityA, entityB) {}
	};

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
			: ContactListener(entity) {}
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
			: ContactListener(GenerateKey(entity1, entity2)) {}
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
			: ContactListener(entity) {}
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
			: ContactListener(GenerateKey(entity1, entity2)) {}
	};

	/**
	* @enum ContactType
	* 
	* @brief Types of contact events.
	*/
	enum ContactType
	{
		BEGIN_CONTACT,	/// Contact began
		END_CONTACT,	/// Contact end
		BEGIN_SENSOR,	/// Sensor began
		END_SENSOR		/// Sensor end
	};

	/**
	* @class ContactSystem
	* 
	* @brief Manages contact events between entities.
	*/
	class ContactSystem
	{
	public:
		using ContactCallback = std::function<void(const Contact&)>; /// Callback function type for contact events

		/**
		* @brief Registers a contact callback for a specific contact type and tile ID.
		* 
		* @param contactType The type of contact event
		* @param tileIdA The tile ID of the first entity
		* @param callback The callback function to register
		*/
		void RegisterContactCallback(ContactType contactType, const size_t tileIdA, ContactCallback callback);

		/**
		* @brief Activates a contact callback for a specific contact type and entities.
		* 
		* @param contactType The type of contact event
		* @param entityA The first entity involved in the contact
		* @param entityB The second entity involved in the contact
		* @param callback The callback function to activate
		*/
		void ActivateContactCallback(ContactType contactType, Entity entityA, Entity entityB, ContactCallback callback);

		/**
		* @brief Registers a contact callback for a specific contact type and tile IDs.
		* 
		* @param contactType The type of contact event
		* @param tileIdA The tile ID of the first entity
		* @param tileIdB The tile ID of the second entity
		* @param callback The callback function to register
		*/
		void RegisterContactCallback(ContactType contactType, const size_t tileIdA, const size_t tileIdB, ContactCallback callback);

		/**
		* @brief Activates a contact callback for a specific contact type and entity.
		* 
		* @param contactType The type of contact event
		* @param entityA The entity involved in the contact
		* @param callback The callback function to activate
		*/
		void ActivateContactCallback(ContactType contactType, Entity entityA, ContactCallback callback);

	public:
		/**
		* @brief Constructor
		* 
		* @param refECS Reference to the ECS
		* @param tileMap Reference to the TileMap
		*/
		ContactSystem(ECS& refECS, TileMap& tileMap);
		~ContactSystem() = default;

		/**
		* @brief Activates all registered contact callbacks.
		*/
		void ActivateContactCallbacks();

		/**
		* @brief Processes all contact events.
		* 
		* @param ptrWorldId Pointer to the world ID
		*/
		void ProcessContacts(void* ptrWorldId);

		/**
		* @brief Cleans up the contact system.
		*/
		void Cleanup();

	private:
		ECS& m_refECS;				/// Reference to the ECS
		TileMap& m_refTileMap;		/// Reference to the TileMap

		std::vector<std::tuple<ContactType, const size_t, ContactCallback>> m_singleEntityContactCallbacks;			/// Single entity contact callbacks
		std::vector<std::tuple<ContactType, const size_t, const size_t, ContactCallback>> m_multiContactCallbacks;	/// Multi-entity contact callbacks

		std::unordered_map<SingleEntityBeginContactKey, ContactCallback> m_beginSingleEntityContactCallbacks;		/// Single entity begin contact callbacks
		std::unordered_map<SingleEntityBeginContactKey, ContactCallback> m_beginSingleEntitySensingCallbacks;		/// Single entity begin sensing callbacks
		std::unordered_map<SingleEntityEndContactKey, ContactCallback> m_endSingleEntityContactCallbacks;			/// Single entity end contact callbacks
		std::unordered_map<SingleEntityEndContactKey, ContactCallback> m_endSingleEntitySensingCallbacks;			/// Single entity end sensing callbacks
		std::unordered_map<MultiEntityBeginContactKey, ContactCallback> m_beginMultiEntityContactCallbacks;			/// Multi-entity begin contact callbacks
		std::unordered_map<MultiEntityBeginContactKey, ContactCallback> m_beginMultiEntitySensingCallbacks;			/// Multi-entity begin sensing callbacks
		std::unordered_map<MultiEntityEndContactKey, ContactCallback> m_endMultiEntityContactCallbacks;				/// Multi-entity end contact callbacks
		std::unordered_map<MultiEntityEndContactKey, ContactCallback> m_endMultiEntitySensingCallbacks;				/// Multi-entity end sensing callbacks
	};
}