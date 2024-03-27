#pragma once

#include <string>

#include "../../include/Layers/Layer.h"
#include "../../include/Logging/Logger.h"
#include "../../include/GameObjects/GameObject.h"
#include "../../include/Layers/LayerEvents.h"
#include "../../include/Layers/ILayerEvent.h"

namespace Engine
{
	Layer::Layer(std::string name, ILayerEvent* ptrLayerEventSystem)
		: m_name(name), m_enabled(true), isAttached(false), ptrLayerEventSystem(ptrLayerEventSystem) {}
	
	Layer::~Layer() {}

	void Layer::addToWorld()
	{
		ENGINE_TRACE("Adding layer {} to the world.", m_name);

		for (GameObject* gameObject : m_gameObjects)
		{
			ptrLayerEventSystem->triggerLayerEventCallback(LayerEvent::AddToWorld, gameObject);
		}
	};

	void Layer::removeFromWorld()
	{
		ENGINE_TRACE("Removing layer {} from the world.", m_name);

		// Need to remove all game objects inside the layer from the world. May need to be careful with how
		// objects are removed due to box2d actively using the object during the world step.
		// Box2d simulating an object that no longer exists will be problematic.

		for (GameObject* gameObject : m_gameObjects)
		{
			ptrLayerEventSystem->triggerLayerEventCallback(LayerEvent::RemoveFromWorld, gameObject);
		}
	};

	void Layer::onAttach() { ENGINE_TRACE("Inside layer {} attach function.", m_name); };
	void Layer::onDetach() { ENGINE_TRACE("Inside layer {} detach function.", m_name); };
	void Layer::onUpdate() { ENGINE_TRACE("Update layer {}", m_name); };
	void Layer::processEvent(Event& e) { ENGINE_TRACE("Process event for layer {}", m_name); };

	const std::string& Layer::getName() const { return m_name; }

	void Layer::addGameObject(GameObject* gameObject)
	{
		if (gameObject->getBodyType() == BodyType::STATIC) {
			auto it = std::find_if(m_gameObjects.begin(), m_gameObjects.end(), [](const GameObject* gameObject) 
				{
					return !gameObject->getBodyType();
				});

			// Insert the static object before the first dynamic object.
			m_gameObjects.insert(it, gameObject);
		}
		else {
			// Add rest of objects to the end of the list.
			m_gameObjects.push_back(gameObject);
		}

		// If the layer is already attached to the worlds layer stack, add the object to the world as well.
		if (isAttached)
		{
			ptrLayerEventSystem->triggerLayerEventCallback(LayerEvent::AddToWorld, gameObject);
		}
	}

	void Layer::removeGameObject(GameObject* gameObject)
	{
		auto it = std::find(m_gameObjects.begin(), m_gameObjects.end(), gameObject);
		if (it != m_gameObjects.end())
		{
			if (isAttached)
			{
				ptrLayerEventSystem->triggerLayerEventCallback(LayerEvent::RemoveFromWorld, gameObject);
			}

			m_gameObjects.erase(it);
		}
	}
}