#pragma once

#include "MathUtil.h"

namespace Engine
{
	using Entity = size_t;

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
			: m_ptrEntityA(entityA), m_ptrEntityB(entityB) {
		}

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
			: m_normalVec(normalVec), Contact(entityA, entityB) {
		}

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
			: Contact(entityA, entityB) {
		}
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
			: Contact(entityA, entityB) {
		}
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
			: Contact(entityA, entityB) {
		}
	};
}