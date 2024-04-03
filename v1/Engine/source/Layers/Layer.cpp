#pragma once

#include <string>

#include "../../include/Layers/Layer.h"
#include "../../include/Logging/Logger.h"
#include "../../include/GameObjects/GameObject.h"
#include "../../include/CallbackSystem/CallbackSystem.h"

namespace Engine
{
	Layer::Layer(std::string name) : ptrICallbackSystem(ICallbackSystem::GetInstance()), 
		m_name(name), m_enabled(true), isAttached(false) {}
	
	Layer::~Layer() {}
	 
	void Layer::AddToWorld()
	{
		ENGINE_TRACE("Adding layer {} to the world.", m_name);

		for (GameObject* gameObject : m_gameObjects)
		{
			ptrICallbackSystem->TriggerCallback(Type::AddToWorld, gameObject);
		}
	};

	void Layer::RemoveFromWorld()
	{
		ENGINE_TRACE("Removing layer {} from the world.", m_name);

		// Need to remove all game objects inside the layer from the world. May need to be careful with how
		// objects are removed due to box2d actively using the object during the world step.
		// Box2d simulating an object that no longer exists will be problematic.

		for (GameObject* gameObject : m_gameObjects)
		{
			ptrICallbackSystem->TriggerCallback(Type::RemoveFromWorld, gameObject);
		}
	};

	void Layer::OnAttach() { ENGINE_TRACE("Inside layer {} attach function.", m_name); };
	void Layer::OnDetach() { ENGINE_TRACE("Inside layer {} detach function.", m_name); };
	void Layer::OnUpdate() { ENGINE_TRACE("Update layer {}", m_name); };
	void Layer::ProcessEvent(Event& e) { ENGINE_TRACE("Process event for layer {}", m_name); };

	const std::string& Layer::GetName() const { return m_name; }

	void Layer::AddGameObject(GameObject* gameObject)
	{
		// Box2d performs better when it simulates static objects before dynamic objects.
		// Collisions are more accurate when static objects are simulated first.
		// Thus, we need to add static objects to the beginning of the list.

		if (gameObject->getBodyType() == BodyType::STATIC) 
		{
			auto it = std::find_if(m_gameObjects.begin(), m_gameObjects.end(), [](const GameObject* gameObject) 
				{
					return !gameObject->getBodyType();
				});

			// Insert the static object before the first dynamic object.
			m_gameObjects.insert(it, gameObject);
		}
		else 
		{
			// Add rest of objects to the end of the list.
			m_gameObjects.push_back(gameObject);
		}

		// If the layer is already attached to the applications layer stack, add the object to the world as well.
		if (isAttached)
		{
			ptrICallbackSystem->TriggerCallback(Type::AddToWorld, gameObject);
		}
	}

	void Layer::RemoveGameObject(GameObject* gameObject)
	{
		auto it = std::find(m_gameObjects.begin(), m_gameObjects.end(), gameObject);
		if (it != m_gameObjects.end())
		{
			if (isAttached)
			{
				ptrICallbackSystem->TriggerCallback(Type::RemoveFromWorld, gameObject);
			}

			m_gameObjects.erase(it);
		}
	}
}