#pragma once

#include "../../include/Physics/ContactSystem.h"
#include "../../include/Logging/Logger.h"
#include <box2d/box2d.h>

namespace Engine
{
	ContactSystem::ContactSystem(ECS& refECS, TileMap& tileMap) : m_refECS(refECS), m_refTileMap(tileMap) {}

	// This function is allocating memory dynamically. Why?
	void ContactSystem::ProcessContacts(void* ptrWorldId)
	{
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
			Math2D::Point2D<float> normal = Math2D::Point2D(beginEvent->manifold.normal.x, beginEvent->manifold.normal.y);

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

	void ContactSystem::ActivateContactCallbacks()
	{
		for (auto& tuple : m_singleEntityContactCallbacks)
		{
			Entity entityA = m_refTileMap.GetEntity(std::get<1>(tuple));
			ContactType contactType = std::get<0>(tuple);
			ContactCallback callback = std::get<2>(tuple);
			if (entityA != m_refECS.INVALID_ENTITY)
			{
				ActivateContactCallback(contactType, entityA, callback);
			}
			else
			{
				ENGINE_CRITICAL_D("Entity does not exist in the ECS. Cannot register contact callback.");
			}
		}

		for (auto& tuple : m_multiContactCallbacks)
		{
			Entity entityA = m_refTileMap.GetEntity(std::get<1>(tuple));
			Entity entityB = m_refTileMap.GetEntity(std::get<2>(tuple));
			ContactType contactType = std::get<0>(tuple);
			ContactCallback callback = std::get<3>(tuple);
			if (entityA != m_refECS.INVALID_ENTITY && entityB != m_refECS.INVALID_ENTITY)
			{
				ActivateContactCallback(contactType, entityA, entityB, callback);
			}
			else
			{
				ENGINE_CRITICAL_D("One or both entities do not exist in the ECS. Cannot register contact callback.");
			}
		}
	}

	void ContactSystem::ActivateContactCallback(ContactType contactType, Entity entityA, ContactCallback callback)
	{
		PhysicsBody* ptrPhysicsBody = m_refECS.GetComponent<PhysicsBody>(entityA);
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

	void ContactSystem::RegisterContactCallback(ContactType contactType, const size_t A, const size_t B, ContactCallback callback)
	{
		m_multiContactCallbacks.emplace_back(contactType, A, B, callback);
	}

	void ContactSystem::RegisterContactCallback(ContactType contactType, const size_t A, ContactCallback callback)
	{
		m_singleEntityContactCallbacks.emplace_back(contactType, A, callback);
	}

	void ContactSystem::ActivateContactCallback(ContactType contactType, Entity entityA, Entity entityB, ContactCallback callback)
	{
		PhysicsBody* ptrPhysicsBodyA = m_refECS.GetComponent<PhysicsBody>(entityA);
		PhysicsBody* ptrPhysicsBodyB = m_refECS.GetComponent<PhysicsBody>(entityB);
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