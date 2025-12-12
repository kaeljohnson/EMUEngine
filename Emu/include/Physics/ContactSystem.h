#pragma once

#include "../ECS/ECS.h"
#include "../Includes.h"
#include "../Core.h"
#include "../Components.h"
#include "../MathUtil.h"
#include "../TileMap/TileMap.h"

#include "ContactKeyHashes.h"

namespace Engine
{
	// Contact wrapper objects
	struct Contact
	{
		Contact(Entity entityA, Entity entityB)
			: m_ptrEntityA(entityA), m_ptrEntityB(entityB) {}

		Entity m_ptrEntityA;
		Entity m_ptrEntityB;
	};

	struct BeginContact : public Contact
	{
		BeginContact(Entity entityA, Entity entityB, Math2D::Point2D<float> normalVec)
			: m_normalVec(normalVec), Contact(entityA, entityB) {}

		Math2D::Point2D<float> m_normalVec;
	};

	struct EndContact : public Contact
	{
		EndContact(Entity entityA, Entity entityB)
			: Contact(entityA, entityB) {}
	};

	struct BeginSensing : public Contact
	{
		BeginSensing(Entity entityA, Entity entityB)
			: Contact(entityA, entityB) {}
	};

	struct EndSensing : public Contact
	{
		EndSensing(Entity entityA, Entity entityB)
			: Contact(entityA, entityB) {}
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
		SingleEntityContactListener(Entity entity)
			: ContactListener(entity) {}
	};

	struct MultiEntityContactListener : public ContactListener
	{
		MultiEntityContactListener(Entity entity1, Entity entity2)
			: ContactListener(GenerateKey(entity1, entity2)) {}
	};

	struct SingleEntitySensorListener : public ContactListener
	{
		SingleEntitySensorListener(Entity entity)
			: ContactListener(entity) {}
	};

	struct MultiEntitySensorListener : public ContactListener
	{
		MultiEntitySensorListener(Entity entity1, Entity entity2)
			: ContactListener(GenerateKey(entity1, entity2)) {}
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


		EMU_API void RegisterContactCallback(ContactType contactType, const char B, ContactCallback callback);
		void ActivateContactCallback(ContactType contactType, Entity entityA, Entity entityB, ContactCallback callback);

		EMU_API void RegisterContactCallback(ContactType contactType, const char A, const char B, ContactCallback callback);
		void ActivateContactCallback(ContactType contactType, Entity entityA, ContactCallback callback);

	public:
		ContactSystem(ECS& refECS, TileMap& tileMap);
		void ActivateContactCallbacks();
		void ProcessContacts(void* ptrWorldId);
		void Cleanup();

	private:
		ECS& m_refECS;
		TileMap& m_refTileMap;

		std::vector<std::tuple<ContactType, const char, ContactCallback>> m_singleEntityContactCallbacks;
		std::vector<std::tuple<ContactType, const char, const char, ContactCallback>> m_multiContactCallbacks;

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