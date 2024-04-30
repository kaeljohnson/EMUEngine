#pragma once

#include <string>

#include "../../include/Events/EventListener.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Scenes/SceneObject.h"
#include "../../include/CallbackSystem/CallbackSystem.h"

namespace Engine
{
	EventListener::EventListener(std::string name) : ptrICallbackSystem(ICallbackSystem::GetInstance()), m_enabled(true), IsAttachedToScene(false), m_name(name) {}
	
	EventListener::~EventListener() 
	{
		// Even if the EventListener is destroyed, the scene objects will still exist.
		// EventListener just organizes the scene objects.
	}

	void EventListener::ProcessEvent(Event& e) { ENGINE_TRACE_D("Process event for EventListener {}", m_name); };

	void EventListener::Subscribe(SceneObject* sceneObject)
	{
		(sceneObject == nullptr) ? ENGINE_WARN_D("SceneObject is nullptr.") : ENGINE_TRACE_D("Adding scene object to EventListener {}.", m_name);

		if (std::find(m_sceneObjects.begin(), m_sceneObjects.end(), sceneObject) != m_sceneObjects.end())
		{
			ENGINE_WARN_D("SceneObject already exists in EventListener {}.", m_name);
			return;
		}

		// Box2d performs better when it simulates static objects before dynamic objects.
		// Collisions are more accurate when static objects are simulated first.
		// Thus, we need to add static objects to the beginning of the list.

		if (sceneObject->GetPhysicsBody()->getBodyType() == BodyType::STATIC)
		{
			auto it = std::find_if(m_sceneObjects.begin(), m_sceneObjects.end(), [&](SceneObject* sceneObject) 
				{
					return !sceneObject->GetPhysicsBody()->getBodyType();
				});

			// Insert the static object before the first dynamic object.
			m_sceneObjects.insert(it, sceneObject);
		}
		else
		{
			// Add rest of objects to the end of the list.
			m_sceneObjects.push_back(sceneObject);
		}
	}

	void EventListener::Unsubscribe(SceneObject* sceneObject)
	{
		(sceneObject == nullptr) ? ENGINE_WARN_D("SceneObject is nullptr.") : ENGINE_TRACE_D("Removing scene object from EventListener {}.", m_name);

		auto ptrSceneObjectIt = std::find(m_sceneObjects.begin(), m_sceneObjects.end(), sceneObject);
		if (ptrSceneObjectIt != m_sceneObjects.end())
		{
			m_sceneObjects.erase(ptrSceneObjectIt);
		}
		else
		{
			ENGINE_WARN_D("SceneObject not found in EventListener {}.", m_name);
		}
	}
}