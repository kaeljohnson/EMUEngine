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

	class ContactSystem
	{
	public:
		using ContactHandler = std::function<void(const Contact&)>; 

		// Should these be in the event system instead?
		EMU_API void RegisterContactListener(SingleEntityContactListener*);
		EMU_API void RegisterContactListener(MultiEntityContactListener*);
		EMU_API void RegisterContactListener(SingleEntitySensorListener*);
		EMU_API void RegisterContactListener(MultiEntitySensorListener*);

		EMU_API void RegisterContactHandler(SingleEntityBeginContactKey key, ContactHandler handler);
		EMU_API void RegisterContactHandler(SingleEntityEndContactKey key, ContactHandler handler);
		EMU_API void RegisterContactHandler(MultiEntityBeginContactKey key, ContactHandler handler);
		EMU_API void RegisterContactHandler(MultiEntityEndContactKey key, ContactHandler handler);

	public:
		ContactSystem(ECS& refECS);
		void ProcessContacts(void* ptrWorldId);
		void Cleanup();

	private:
		ECS& m_refECS;

		std::unordered_map<size_t, SingleEntityContactListener*> m_singleEntityContactListeners;
		std::unordered_map<size_t, MultiEntityContactListener*> m_multiEntityContactListeners;
		std::unordered_map<size_t, SingleEntitySensorListener*> m_singleEntitySensorListeners;
		std::unordered_map<size_t, MultiEntitySensorListener*> m_multiEntitySensorListeners;

		std::unordered_map<SingleEntityBeginContactKey, ContactHandler> m_beginContactHandlers;
		std::unordered_map<SingleEntityEndContactKey, ContactHandler> m_endContactHandlers;
		std::unordered_map<MultiEntityBeginContactKey, ContactHandler> m_multiContactHandlers;
		std::unordered_map<MultiEntityEndContactKey, ContactHandler> m_multiEndContactHandlers;
	};
}