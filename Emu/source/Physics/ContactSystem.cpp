#pragma once

#include "../../include/Physics/ContactSystem.h"
#include <box2d/box2d.h>

namespace Engine
{

	std::unordered_map<size_t, SingleEntityContactListener*> ContactSystem::m_singleEntityContactListeners;
	std::unordered_map<size_t, MultiEntityContactListener*> ContactSystem::m_multiEntityContactListeners;
	std::unordered_map<size_t, SingleEntitySensorListener*> ContactSystem::m_singleEntitySensorListeners;
	std::unordered_map<size_t, MultiEntitySensorListener*> ContactSystem::m_multiEntitySensorListeners;

	std::unordered_map<SingleEntityBeginContactKey, std::function<void(const Contact&)>> ContactSystem::m_beginContactHandlers;
	std::unordered_map<SingleEntityEndContactKey, std::function<void(const Contact&)>> ContactSystem::m_endContactHandlers;
	std::unordered_map<MultiEntityBeginContactKey, std::function<void(const Contact&)>> ContactSystem::m_multiContactHandlers;
	std::unordered_map<MultiEntityEndContactKey, std::function<void(const Contact&)>> ContactSystem::m_multiEndContactHandlers;

	void ContactSystem::ProcessContacts(void* ptrWorldId)
	{
		// Process ContactComponents
		for (SimpleContact& simpleContact : ECS::GetComponentManager<SimpleContact>())
		{
			simpleContact.m_contactAbove = false;
			simpleContact.m_contactBelow = false;
			simpleContact.m_contactLeft = false;
			simpleContact.m_contactRight = false;

			// Better way to access. Maybe can just store shapeId directly in SimpleContact component?
			Entity* ptrEntity = simpleContact.GetEntity();
			b2ShapeId* shapeId = ECS::GetComponentManager<PhysicsBody>().GetComponent(ptrEntity)->m_shapeId;

			b2ContactData contactData[10];
			int shapeContactCount = b2Shape_GetContactData(*shapeId, contactData, 10);

			for (int i = 0; i < shapeContactCount; ++i)
			{
				b2ContactData* contact = contactData + i;

				float normalDirection = 1.0f;

				if ((Entity*)b2Body_GetUserData(b2Shape_GetBody(contact->shapeIdB)) == ptrEntity)
				{
					normalDirection = -1.0f;
				}

				b2Vec2 normal = contact->manifold.normal * normalDirection;

				if (normal.y < -0.5) // Collision from above `this`
				{
					// ENGINE_CRITICAL_D("Contact Above!");
					simpleContact.m_contactAbove = true;
				}
				else if (normal.y > 0.5) // Collision from below `this`
				{
					// ENGINE_CRITICAL_D("Contact Below!");
					simpleContact.m_contactBelow = true;
				}

				if (normal.x > 0.5) // Collision from the Right of `this`
				{
					// ENGINE_CRITICAL_D("Contact Right!");
					simpleContact.m_contactRight = true;
				}
				else if (normal.x < -0.5) // Collision from the Left of `this`
				{
					// ENGINE_CRITICAL_D("Contact Left!");
					simpleContact.m_contactLeft = true;
				}
			}
		}

		// May need to ensure that only one contact event is created per contact. 
		// So that we don't have multiple events for the same contact.
		b2ContactEvents contactEvents = b2World_GetContactEvents(*(b2WorldId*)ptrWorldId);

		for (int i = 0; i < contactEvents.beginCount; ++i)
		{
			b2ContactBeginTouchEvent* beginEvent = contactEvents.beginEvents + i;

			b2ShapeId shapeIdA = beginEvent->shapeIdA;
			b2ShapeId shapeIdB = beginEvent->shapeIdB;

			Entity* entityA = (Entity*)b2Body_GetUserData(b2Shape_GetBody(shapeIdA));
			Entity* entityB = (Entity*)b2Body_GetUserData(b2Shape_GetBody(shapeIdB));

			// normal points from A to B
			Vector2D<float> normal = Vector2D(beginEvent->manifold.normal.x, beginEvent->manifold.normal.y);

			// Process Listeners
			auto beginSingleContactIteratorA = m_singleEntityContactListeners.find(entityA->GetID());
			if (beginSingleContactIteratorA != m_singleEntityContactListeners.end())
			{
				beginSingleContactIteratorA->second->OnContactBegin(BeginContact(entityA, entityB, normal));
			}

			auto beginSingleContactIteratorB = m_singleEntityContactListeners.find(entityB->GetID());
			if (beginSingleContactIteratorB != m_singleEntityContactListeners.end())
			{
				beginSingleContactIteratorB->second->OnContactBegin(BeginContact(entityB, entityA, normal * -1.0f));
			}

			auto multiContactIterator = m_multiEntityContactListeners.find(GenerateKey(entityA->GetID(), entityB->GetID()));
			if (multiContactIterator != m_multiEntityContactListeners.end())
			{
				multiContactIterator->second->OnContactBegin(BeginContact(entityA, entityB, normal));
			}

			// Process Event Handlers
			auto singleContactEventHandlerA = m_beginContactHandlers.find(SingleEntityBeginContactKey(entityA));
			if (singleContactEventHandlerA != m_beginContactHandlers.end())
			{
				singleContactEventHandlerA->second(BeginContact(entityA, entityB, normal));
			}

			auto singleContactEventHandlerB = m_beginContactHandlers.find(SingleEntityBeginContactKey(entityB));
			if (singleContactEventHandlerB != m_beginContactHandlers.end())
			{
				singleContactEventHandlerB->second(BeginContact(entityB, entityA, normal * -1.0f));
			}

			auto multiContactEventHandler = m_multiContactHandlers.find(MultiEntityBeginContactKey(entityA, entityB));
			if (multiContactEventHandler != m_multiContactHandlers.end())
			{
				multiContactEventHandler->second(BeginContact(entityA, entityB, normal));
			}
		}

		for (int i = 0; i < contactEvents.endCount; ++i)
		{
			b2ContactEndTouchEvent* endEvent = contactEvents.endEvents + i;

			b2ShapeId shapeIdA = endEvent->shapeIdA;
			b2ShapeId shapeIdB = endEvent->shapeIdB;

			Entity* entityA = (Entity*)b2Body_GetUserData(b2Shape_GetBody(shapeIdA));
			Entity* entityB = (Entity*)b2Body_GetUserData(b2Shape_GetBody(shapeIdB));

			// Process Listeners
			auto endSingleContactIteratorA = m_singleEntityContactListeners.find(entityA->GetID());
			if (endSingleContactIteratorA != m_singleEntityContactListeners.end())
			{
				endSingleContactIteratorA->second->OnContactEnd(EndContact(entityA, entityB));
			}

			auto endSingleContactIteratorB = m_singleEntityContactListeners.find(entityB->GetID());
			if (endSingleContactIteratorB != m_singleEntityContactListeners.end())
			{
				endSingleContactIteratorB->second->OnContactEnd(EndContact(entityB, entityA));
			}

			auto endMultiContactIterator = m_multiEntityContactListeners.find(GenerateKey(entityA->GetID(), entityB->GetID()));
			if (endMultiContactIterator != m_multiEntityContactListeners.end())
			{
				endMultiContactIterator->second->OnContactEnd(EndContact(entityA, entityB));
			}

			// Process handlers
			auto singleContactEventHandlerA = m_endContactHandlers.find(SingleEntityEndContactKey(entityA));
			if (singleContactEventHandlerA != m_endContactHandlers.end())
			{
				singleContactEventHandlerA->second(EndContact(entityA, entityB));
			}

			auto singleContactEventHandlerB = m_endContactHandlers.find(SingleEntityEndContactKey(entityB));
			if (singleContactEventHandlerB != m_endContactHandlers.end())
			{
				singleContactEventHandlerB->second(EndContact(entityB, entityA));
			}

			auto multiContactEventHandler = m_multiEndContactHandlers.find(MultiEntityEndContactKey(entityA, entityB));
			if (multiContactEventHandler != m_multiEndContactHandlers.end())
			{
				multiContactEventHandler->second(EndContact(entityA, entityB));
			}
		}

		///*for (int i = 0; i < contactEvents.hitCount; ++i)
		//{
		//	b2ContactHitEvent* hitEvent = contactEvents.hitEvents + i;\
			//}*/

		b2SensorEvents sensorEvents = b2World_GetSensorEvents(*(b2WorldId*)ptrWorldId);

		for (int i = 0; i < sensorEvents.beginCount; ++i)
		{
			b2SensorBeginTouchEvent* beginEvent = sensorEvents.beginEvents + i;

			b2ShapeId shapeIdA = beginEvent->visitorShapeId;
			b2ShapeId shapeIdB = beginEvent->sensorShapeId;

			Entity* entityA = (Entity*)b2Body_GetUserData(b2Shape_GetBody(shapeIdA));
			Entity* entityB = (Entity*)b2Body_GetUserData(b2Shape_GetBody(shapeIdB));

			// Process Listeners
			auto beginSingleSensingIteratorA = m_singleEntitySensorListeners.find(entityA->GetID());
			if (beginSingleSensingIteratorA != m_singleEntitySensorListeners.end())
			{
				beginSingleSensingIteratorA->second->OnContactBegin(BeginSensing(entityA, entityB));
			}

			auto beingSingleSensingIteratorB = m_singleEntitySensorListeners.find(entityB->GetID());
			if (beingSingleSensingIteratorB != m_singleEntitySensorListeners.end())
			{
				beingSingleSensingIteratorB->second->OnContactBegin(BeginSensing(entityB, entityA));
			}

			auto beginMultiSensingIterator = m_multiEntitySensorListeners.find(GenerateKey(entityA->GetID(), entityB->GetID()));
			if (beginMultiSensingIterator != m_multiEntitySensorListeners.end())
			{
				beginMultiSensingIterator->second->OnContactBegin(BeginSensing(entityA, entityB));
			}

			// Process Event Handlers
			auto singleSensorEventHandlerA = m_beginContactHandlers.find(SingleEntityBeginContactKey(entityA));
			if (singleSensorEventHandlerA != m_beginContactHandlers.end())
			{
				singleSensorEventHandlerA->second(BeginSensing(entityA, entityB));
			}

			auto singleSensorEventHandlerB = m_beginContactHandlers.find(SingleEntityBeginContactKey(entityB));
			if (singleSensorEventHandlerB != m_beginContactHandlers.end())
			{
				singleSensorEventHandlerB->second(BeginSensing(entityB, entityA));
			}

			auto multiSensorEventHandler = m_multiContactHandlers.find(MultiEntityBeginContactKey(entityA, entityB));
			if (multiSensorEventHandler != m_multiContactHandlers.end())
			{
				multiSensorEventHandler->second(BeginSensing(entityA, entityB));
			}
		}

		for (int i = 0; i < sensorEvents.endCount; ++i)
		{
			b2SensorEndTouchEvent* endEvent = sensorEvents.endEvents + i;

			b2ShapeId shapeIdA = endEvent->visitorShapeId;
			b2ShapeId shapeIdB = endEvent->sensorShapeId;

			Entity* entityA = (Entity*)b2Body_GetUserData(b2Shape_GetBody(shapeIdA));
			Entity* entityB = (Entity*)b2Body_GetUserData(b2Shape_GetBody(shapeIdB));

			// Process Listeners
			auto endSingleSensingIterator = m_singleEntitySensorListeners.find(entityA->GetID());
			if (endSingleSensingIterator != m_singleEntitySensorListeners.end())
			{
				endSingleSensingIterator->second->OnContactEnd(EndSensing(entityA, entityB));
			}

			auto endSingleSensingIteratorB = m_singleEntitySensorListeners.find(entityB->GetID());
			if (endSingleSensingIteratorB != m_singleEntitySensorListeners.end())
			{
				endSingleSensingIteratorB->second->OnContactEnd(EndSensing(entityB, entityA));
			}

			auto endMultiSensingIterator = m_multiEntitySensorListeners.find(GenerateKey(entityA->GetID(), entityB->GetID()));
			if (endMultiSensingIterator != m_multiEntitySensorListeners.end())
			{
				endMultiSensingIterator->second->OnContactEnd(EndSensing(entityA, entityB));
			}

			// Process Event Handlers
			auto singleSensorEventHandlerA = m_endContactHandlers.find(SingleEntityEndContactKey(entityA));
			if (singleSensorEventHandlerA != m_endContactHandlers.end())
			{
				singleSensorEventHandlerA->second(EndSensing(entityA, entityB));
			}

			auto singleSensorEventHandlerB = m_endContactHandlers.find(SingleEntityEndContactKey(entityB));
			if (singleSensorEventHandlerB != m_endContactHandlers.end())
			{
				singleSensorEventHandlerB->second(EndSensing(entityB, entityA));
			}

			auto multiSensorEventHandler = m_multiEndContactHandlers.find(MultiEntityEndContactKey(entityA, entityB));
			if (multiSensorEventHandler != m_multiEndContactHandlers.end())
			{
				multiSensorEventHandler->second(EndSensing(entityA, entityB));
			}
		}
	}

	void ContactSystem::RegisterContactListener(SingleEntityContactListener* listener)
	{
		m_singleEntityContactListeners.emplace(listener->GetKey(), listener);
	}

	void ContactSystem::RegisterContactListener(MultiEntityContactListener* listener)
	{
		m_multiEntityContactListeners.emplace(listener->GetKey(), listener);
	}

	void ContactSystem::RegisterContactListener(SingleEntitySensorListener* listener)
	{
		m_singleEntitySensorListeners.emplace(listener->GetKey(), listener);
	}

	void ContactSystem::RegisterContactListener(MultiEntitySensorListener* listener)
	{
		m_multiEntitySensorListeners.emplace(listener->GetKey(), listener);
	}

	void ContactSystem::RegisterContactHandler(SingleEntityBeginContactKey key, ContactHandler handler)
	{
		m_beginContactHandlers.emplace(key, handler);
	}

	void ContactSystem::RegisterContactHandler(SingleEntityEndContactKey key, ContactHandler handler)
	{
		m_endContactHandlers.emplace(key, handler);
	}

	void ContactSystem::RegisterContactHandler(MultiEntityBeginContactKey key, ContactHandler handler)
	{
		m_multiContactHandlers.emplace(key, handler);
	}

	void ContactSystem::RegisterContactHandler(MultiEntityEndContactKey key, ContactHandler handler)
	{
		m_multiEndContactHandlers.emplace(key, handler);
	}

	void ContactSystem::Cleanup()
	{
		m_singleEntityContactListeners.clear();
		m_multiEntityContactListeners.clear();
		m_singleEntitySensorListeners.clear();
		m_multiEntitySensorListeners.clear();

		m_beginContactHandlers.clear();
		m_endContactHandlers.clear();
		m_multiContactHandlers.clear();
		m_multiEndContactHandlers.clear();
	}
}