#pragma once

#include <string>

#include "../../include/Layers/Layer.h"
#include "../../include/Logging/Logger.h"
#include "../../include/SceneObjects/SceneObject.h"
#include "../../include/CallbackSystem/CallbackSystem.h"

namespace Engine
{
	Layer::Layer(std::string name) : ptrICallbackSystem(ICallbackSystem::GetInstance()),
		m_name(name), m_enabled(true), IsAttachedToScene(false) {}
	
	Layer::~Layer() 
	{
		ENGINE_TRACE_D("Destroying layer {}.", m_name);
		// Even if the layer is destroyed, the scene objects will still exist.
		// Layer just organizes the scene objects.
	}
	 
	void Layer::AddToWorld()
	{
		ENGINE_TRACE_D("Adding layer {} to the world.", m_name);

		for (SceneObject* sceneObject : m_sceneObjects)
		{
			ptrICallbackSystem->TriggerCallback(Type::AddToWorld, sceneObject);
		}
	};

	void Layer::RemoveFromWorld()
	{
		ENGINE_TRACE_D("Removing layer {} from the world.", m_name);

		// Need to remove all scene objects inside the layer from the world. May need to be careful with how
		// objects are removed due to box2d actively using the object during the world step.
		// Box2d simulating an object that no longer exists will be problematic.

		for (SceneObject* sceneObject : m_sceneObjects)
		{
			sceneObject->GetPhysicsBody()->removeBodyFromWorld();
		}
	};

	void Layer::OnAttach() { ENGINE_TRACE_D("Inside layer {} attach function.", m_name); };
	void Layer::OnDetach() { ENGINE_TRACE_D("Inside layer {} detach function.", m_name); };
	void Layer::Free() 
	{ 
		ENGINE_TRACE_D("Freeing layer {}.", m_name); 
		// May need to free some resources? 
	};
	void Layer::OnUpdate() { ENGINE_TRACE_D("Update layer {}", m_name); };
	void Layer::ProcessEvent(Event& e) { ENGINE_TRACE_D("Process event for layer {}", m_name); };

	const std::string& Layer::GetName() const { return m_name; }

	void Layer::AddSceneObject(SceneObject* sceneObject)
	{
		(sceneObject == nullptr) ? ENGINE_WARN_D("SceneObject is nullptr.") : ENGINE_TRACE_D("Adding scene object to layer {}.", m_name);

		if (std::find(m_sceneObjects.begin(), m_sceneObjects.end(), sceneObject) != m_sceneObjects.end())
		{
			ENGINE_WARN_D("SceneObject already exists in layer {}.", m_name);
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

		// If the layer is already attached to the scenes layer stack, add the object to the world as well.
		if (IsAttachedToScene)
		{
			ptrICallbackSystem->TriggerCallback(Type::AddToWorld, sceneObject);
		}
	}

	void Layer::RemoveSceneObject(SceneObject* sceneObject)
	{
		(sceneObject == nullptr) ? ENGINE_WARN_D("SceneObject is nullptr.") : ENGINE_TRACE_D("Removing scene object from layer {}.", m_name);

		auto ptrSceneObjectIt = std::find(m_sceneObjects.begin(), m_sceneObjects.end(), sceneObject);
		if (ptrSceneObjectIt != m_sceneObjects.end())
		{
			if (IsAttachedToScene)
			{
				sceneObject->GetPhysicsBody()->removeBodyFromWorld();
			}

			m_sceneObjects.erase(ptrSceneObjectIt);
		}
		else
		{
			ENGINE_WARN_D("SceneObject not found in layer {}.", m_name);
		}
	}
}