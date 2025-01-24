#pragma once

#include "../ECS/Entity.h"
#include "../ECS/ECS.h"
#include "../Includes.h"
#include "../Core.h"
#include "../Components.h"
#include "../MathUtil.h"

#include "ContactKeyHashes.h"

namespace Engine
{
	// Contact wrapper objects
	struct Contact
	{
		Contact(Entity* ptrEntityA, Entity* ptrEntityB)
			: m_ptrEntityA(ptrEntityA), m_ptrEntityB(ptrEntityB) {}

		Entity* m_ptrEntityA;
		Entity* m_ptrEntityB;
	};

	struct BeginContact : public Contact
	{
		BeginContact(Entity* ptrEntityA, Entity* ptrEntityB, Vector2D<float> normalVec)
			: m_normalVec(normalVec), Contact(ptrEntityA, ptrEntityB) {}

		Vector2D<float> m_normalVec;
	};

	struct EndContact : public Contact
	{
		EndContact(Entity* ptrEntityA, Entity* ptrEntityB)
			: Contact(ptrEntityA, ptrEntityB) {}
	};

	struct BeginSensing : public Contact
	{
		BeginSensing(Entity* ptrEntityA, Entity* ptrEntityB)
			: Contact(ptrEntityA, ptrEntityB) {}
	};

	struct EndSensing : public Contact
	{
		EndSensing(Entity* ptrEntityA, Entity* ptrEntityB)
			: Contact(ptrEntityA, ptrEntityB) {}
	};

	struct ContactListener
	{
		ContactListener(size_t key) : m_key(key) {}

		virtual ~ContactListener() = default;

		virtual void OnContactBegin(const Contact event) {};
		virtual void OnContactEnd(const Contact event) {};

		size_t GetKey() const { return m_key; }

	private:
		const size_t m_key;
	};

	// Default contact listeners
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

	enum ContactType
	{
		BEGIN_CONTACT,
		END_CONTACT,
		BEGIN_SENSOR,
		END_SENSOR
	};

	class ContactSystem
	{
	public:
		using ContactCallback = std::function<void(const Contact&)>; 


		EMU_API void RegisterContactCallback(ContactType contactType, Entity* ptrEntityA, Entity* ptrEntityB, ContactCallback callback);
		EMU_API void RegisterContactCallback(ContactType contactType, Entity* ptrEntity, ContactCallback callback);

	public:
		ContactSystem(ECS& refECS);
		void ProcessContacts(void* ptrWorldId);
		void Cleanup();

	private:
		ECS& m_refECS;

		std::unordered_map<SingleEntityBeginContactKey, ContactCallback> m_beginSingleEntityContactCallbacks;
		std::unordered_map<SingleEntityBeginContactKey, ContactCallback> m_beginSingleEntitySensingCallbacks;
		std::unordered_map<SingleEntityEndContactKey, ContactCallback> m_endSingleEntityContactCallbacks;
		std::unordered_map<SingleEntityEndContactKey, ContactCallback> m_endSingleEntitySensingCallbacks;
		std::unordered_map<MultiEntityBeginContactKey, ContactCallback> m_beginMultiEntityContactCallbacks;
		std::unordered_map<MultiEntityBeginContactKey, ContactCallback> m_beginMultiEntitySensingCallbacks;
		std::unordered_map<MultiEntityEndContactKey, ContactCallback> m_endMultiEntityContactCallbacks;
		std::unordered_map<MultiEntityEndContactKey, ContactCallback> m_endMultiEntitySensingCallbacks;
	};
}