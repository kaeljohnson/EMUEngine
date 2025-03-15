#pragma once

#include "../../include/Physics/ContactSystem.h"
#include "../../include/Logging/Logger.h"
#include <box2d/box2d.h>

namespace Engine
{
	ContactSystem::ContactSystem(ECS& refECS) : m_refECS(refECS) {}

	void ContactSystem::ProcessContacts(void* ptrWorldId)
	{
		// Process ContactComponents
		for (SimpleContact& simpleContact : m_refECS.GetComponentManager<SimpleContact>())
		{
			simpleContact.m_contactAbove = false;
			simpleContact.m_contactBelow = false;
			simpleContact.m_contactLeft = false;
			simpleContact.m_contactRight = false;

			// Better way to access. Maybe can just store shapeId directly in SimpleContact component?
			Entity entity = simpleContact.m_entity;
			b2ShapeId* shapeId = m_refECS.GetComponentManager<PhysicsBody>().GetComponent(entity)->m_shapeId;

			b2ContactData contactData[10];
			int shapeContactCount = b2Shape_GetContactData(*shapeId, contactData, 10);

			for (int i = 0; i < shapeContactCount; ++i)
			{
				b2ContactData* contact = contactData + i;

				float normalDirection = 1.0f;

				if ((Entity)b2Body_GetUserData(b2Shape_GetBody(contact->shapeIdB)) == entity)
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

			Entity entityA = (Entity)b2Body_GetUserData(b2Shape_GetBody(shapeIdA));
			Entity entityB = (Entity)b2Body_GetUserData(b2Shape_GetBody(shapeIdB));

			// normal points from A to B
			Vector2D<float> normal = Vector2D(beginEvent->manifold.normal.x, beginEvent->manifold.normal.y);

			// Process Event Handlers
			auto singleContactEventHandlerA = m_beginSingleEntityContactCallbacks.find(SingleEntityBeginContactKey(entityA));
			if (singleContactEventHandlerA != m_beginSingleEntityContactCallbacks.end())
			{
				singleContactEventHandlerA->second(BeginContact(entityA, entityB, normal));
			}

			auto singleContactEventHandlerB = m_beginSingleEntityContactCallbacks.find(SingleEntityBeginContactKey(entityB));
			if (singleContactEventHandlerB != m_beginSingleEntityContactCallbacks.end())
			{
				singleContactEventHandlerB->second(BeginContact(entityB, entityA, normal * -1.0f));
			}

			auto multiContactEventHandler = m_beginMultiEntityContactCallbacks.find(MultiEntityBeginContactKey(entityA, entityB));
			if (multiContactEventHandler != m_beginMultiEntityContactCallbacks.end())
			{
				multiContactEventHandler->second(BeginContact(entityA, entityB, normal));
			}
		}

		for (int i = 0; i < contactEvents.endCount; ++i)
		{
			b2ContactEndTouchEvent* endEvent = contactEvents.endEvents + i;

			b2ShapeId shapeIdA = endEvent->shapeIdA;
			b2ShapeId shapeIdB = endEvent->shapeIdB;

			Entity entityA = (Entity)b2Body_GetUserData(b2Shape_GetBody(shapeIdA));
			Entity entityB = (Entity)b2Body_GetUserData(b2Shape_GetBody(shapeIdB));

			// Process handlers
			auto singleContactEventHandlerA = m_endSingleEntityContactCallbacks.find(SingleEntityEndContactKey(entityA));
			if (singleContactEventHandlerA != m_endSingleEntityContactCallbacks.end())
			{
				singleContactEventHandlerA->second(EndContact(entityA, entityB));
			}

			auto singleContactEventHandlerB = m_endSingleEntityContactCallbacks.find(SingleEntityEndContactKey(entityB));
			if (singleContactEventHandlerB != m_endSingleEntityContactCallbacks.end())
			{
				singleContactEventHandlerB->second(EndContact(entityB, entityA));
			}

			auto multiContactEventHandler = m_endMultiEntityContactCallbacks.find(MultiEntityEndContactKey(entityA, entityB));
			if (multiContactEventHandler != m_endMultiEntityContactCallbacks.end())
			{
				multiContactEventHandler->second(EndContact(entityA, entityB));
			}
		}

		b2SensorEvents sensorEvents = b2World_GetSensorEvents(*(b2WorldId*)ptrWorldId);

		for (int i = 0; i < sensorEvents.beginCount; ++i)
		{
			b2SensorBeginTouchEvent* beginEvent = sensorEvents.beginEvents + i;

			b2ShapeId shapeIdA = beginEvent->visitorShapeId;
			b2ShapeId shapeIdB = beginEvent->sensorShapeId;

			Entity entityA = (Entity)b2Body_GetUserData(b2Shape_GetBody(shapeIdA));
			Entity entityB = (Entity)b2Body_GetUserData(b2Shape_GetBody(shapeIdB));

			// Process Event Handlers
			auto singleSensorEventHandlerA = m_beginSingleEntitySensingCallbacks.find(SingleEntityBeginContactKey(entityA));
			if (singleSensorEventHandlerA != m_beginSingleEntitySensingCallbacks.end())
			{
				singleSensorEventHandlerA->second(BeginSensing(entityA, entityB));
			}

			auto singleSensorEventHandlerB = m_beginSingleEntitySensingCallbacks.find(SingleEntityBeginContactKey(entityB));
			if (singleSensorEventHandlerB != m_beginSingleEntitySensingCallbacks.end())
			{
				singleSensorEventHandlerB->second(BeginSensing(entityB, entityA));
			}

			auto multiSensorEventHandler = m_beginMultiEntitySensingCallbacks.find(MultiEntityBeginContactKey(entityA, entityB));
			if (multiSensorEventHandler != m_beginMultiEntitySensingCallbacks.end())
			{
				multiSensorEventHandler->second(BeginSensing(entityA, entityB));
			}
		}

		for (int i = 0; i < sensorEvents.endCount; ++i)
		{
			b2SensorEndTouchEvent* endEvent = sensorEvents.endEvents + i;

			b2ShapeId shapeIdA = endEvent->visitorShapeId;
			b2ShapeId shapeIdB = endEvent->sensorShapeId;

			Entity entityA = (Entity)b2Body_GetUserData(b2Shape_GetBody(shapeIdA));
			Entity entityB = (Entity)b2Body_GetUserData(b2Shape_GetBody(shapeIdB));


			// Process Event Handlers
			auto singleSensorEventHandlerA = m_endSingleEntitySensingCallbacks.find(SingleEntityEndContactKey(entityA));
			if (singleSensorEventHandlerA != m_endSingleEntitySensingCallbacks.end())
			{
				singleSensorEventHandlerA->second(EndSensing(entityA, entityB));
			}

			auto singleSensorEventHandlerB = m_endSingleEntitySensingCallbacks.find(SingleEntityEndContactKey(entityB));
			if (singleSensorEventHandlerB != m_endSingleEntitySensingCallbacks.end())
			{
				singleSensorEventHandlerB->second(EndSensing(entityB, entityA));
			}

			auto multiSensorEventHandler = m_endMultiEntitySensingCallbacks.find(MultiEntityEndContactKey(entityA, entityB));
			if (multiSensorEventHandler != m_endMultiEntitySensingCallbacks.end())
			{
				multiSensorEventHandler->second(EndSensing(entityA, entityB));
			}
		}
	}

	void ContactSystem::RegisterContactCallback(ContactType contactType, Entity entityA, ContactCallback callback)
	{
		PhysicsBody* ptrPhysicsBody = m_refECS.GetComponentManager<PhysicsBody>().GetComponent(entityA);
		if (ptrPhysicsBody == nullptr)
		{
			ENGINE_CRITICAL_D("Entity does not have a PhysicsBody component. Cannot register contact callback.");
			return;
		}
		
		if (ptrPhysicsBody->m_bodyType == BodyType::SENSOR && (contactType == BEGIN_CONTACT || contactType == END_CONTACT))
		{
			ENGINE_CRITICAL_D("Entity is a sensor. Cannot register contact callback. Register a sensor callback instead.");
			return;
		}

		if (ptrPhysicsBody->m_bodyType != BodyType::SENSOR && (contactType == BEGIN_SENSOR || contactType == END_SENSOR))
		{
			ENGINE_CRITICAL_D("Entity is not a sensor. Cannot register sensor callback. Register a contact callback instead.");
			return;
		}

		switch (contactType)
		{
		case BEGIN_CONTACT:
			m_beginSingleEntityContactCallbacks.emplace(SingleEntityBeginContactKey(entityA), callback);
			break;
		case END_CONTACT:
			m_endSingleEntityContactCallbacks.emplace(SingleEntityEndContactKey(entityA), callback);
			break;
		case BEGIN_SENSOR:
			m_beginSingleEntitySensingCallbacks.emplace(SingleEntityBeginContactKey(entityA), callback);
			break;
		case END_SENSOR:
			m_endSingleEntitySensingCallbacks.emplace(SingleEntityEndContactKey(entityA), callback);
			break;
		}
	}

	void ContactSystem::RegisterContactCallback(ContactType contactType, Entity entityA, Entity entityB, ContactCallback callback)
	{
		PhysicsBody* ptrPhysicsBodyA = m_refECS.GetComponentManager<PhysicsBody>().GetComponent(entityA);
		PhysicsBody* ptrPhysicsBodyB = m_refECS.GetComponentManager<PhysicsBody>().GetComponent(entityB);
		if (ptrPhysicsBodyA == nullptr || ptrPhysicsBodyB == nullptr)
		{
			ENGINE_CRITICAL_D("One or both entities do not have a PhysicsBody component. Cannot register contact callback.");
			return;
		}
		else if ((ptrPhysicsBodyA->m_bodyType == BodyType::SENSOR || ptrPhysicsBodyB->m_bodyType == BodyType::SENSOR) 
			&& (contactType == BEGIN_CONTACT || contactType == END_CONTACT))
		{
			ENGINE_CRITICAL_D("One or both entities are sensors. Cannot register contact callback. Register a sensor callback instead.");
			return;
		}
		else if ((ptrPhysicsBodyA->m_bodyType != BodyType::SENSOR && ptrPhysicsBodyB->m_bodyType != BodyType::SENSOR) 
			&& (contactType == BEGIN_SENSOR || contactType == END_SENSOR))
		{
			ENGINE_CRITICAL_D("Neither entity are sensors. Cannot register Sensor callback. Register a Contact callback instead.");
			return;
		}

		switch (contactType)
		{
		case BEGIN_CONTACT:
			m_beginMultiEntityContactCallbacks.emplace(MultiEntityBeginContactKey(entityA, entityB), callback);
			break;
		case END_CONTACT:
			m_endMultiEntityContactCallbacks.emplace(MultiEntityEndContactKey(entityA, entityB), callback);
			break;
		case BEGIN_SENSOR:
			m_beginMultiEntitySensingCallbacks.emplace(MultiEntityBeginContactKey(entityA, entityB), callback);
			break;
		case END_SENSOR:
			m_endMultiEntitySensingCallbacks.emplace(MultiEntityEndContactKey(entityA, entityB), callback);
			break;
		}
	}

	void ContactSystem::Cleanup()
	{
		m_beginSingleEntityContactCallbacks.clear();
		m_endSingleEntityContactCallbacks.clear();
		m_beginSingleEntitySensingCallbacks.clear();
		m_endSingleEntitySensingCallbacks.clear();

		m_beginMultiEntityContactCallbacks.clear();
		m_endMultiEntityContactCallbacks.clear();
		m_beginMultiEntitySensingCallbacks.clear();
		m_endMultiEntitySensingCallbacks.clear();
	}
}