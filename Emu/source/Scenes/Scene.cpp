#pragma once

#include <string>

#include "../../include/Scenes/Scene.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Physics/Box.h"
#include "../../include/Physics/PhysicsFactory.h"
#include "../../include/Logging/Logger.h"
#include "../../include/CallbackSystem/CallbackSystem.h"

namespace Engine
{
	Scene::Scene(std::string name, const float timestep, const int pixelsPerMeter) : m_name(name), m_layerStack(), m_pixelsPerMeter(pixelsPerMeter), m_timeStep(timestep)
	{
		m_world = CreateWorld(0.0f * m_pixelsPerMeter, 9.8f * m_pixelsPerMeter, m_timeStep, 8, 3);

		ICallbackSystem::GetInstance()->NewCallback(Type::AddToWorld, [this](Data layerEventData)
			{
				SceneObject* sceneObject = std::get<SceneObject*>(layerEventData);
				Box* ptrBox = static_cast<Box*>(sceneObject->GetPhysicsBody());

				m_world->addBox(ptrBox);
			});
	}

	Scene::~Scene()
	{
		delete m_world;
		m_world = nullptr;
	}

	void Scene::update()
	{
		// Faster way to do this? Should only have to update objects
		// prev values if they have changed. In fact, should only update
		// objects that have changed in general

		for (Layer* layer : m_layerStack)
		{
			for (SceneObject* sceneObject : *layer)
			{
				sceneObject->GetPhysicsBody()->updatePrevX();
				sceneObject->GetPhysicsBody()->updatePrevY();
			}
		}
		m_world->update();
	};

	// Adds the layer to the world. 
	void Scene::AddToWorld(Layer* layer)
	{
		ENGINE_TRACE_D("Adding layer {} to the scenes world.", m_name);

		for (SceneObject* sceneObject : *layer)
		{
			m_world->addBox(static_cast<Box*>(sceneObject->GetPhysicsBody()));
		}
	}

	// Removes the layer from the world.
	void Scene::RemoveFromWorld(Layer* layer)
	{
		ENGINE_TRACE_D("Removing layer {} from the scenes world.", m_name);

		// Need to remove all scene objects inside the layer from the world. May need to be careful with how
		// objects are removed due to box2d actively using the object during the world step.
		// Box2d simulating an object that no longer exists will be problematic.

		for (SceneObject* sceneObject : *layer)
		{
			m_world->removeBox(static_cast<Box*>(sceneObject->GetPhysicsBody()));
		}
	}

	void Scene::SetSimulation(const float gravityX, const float gravityY)
	{
		m_world->SetGravity(gravityX * m_pixelsPerMeter, gravityY * m_pixelsPerMeter);
		m_world->SetTimeStep(m_timeStep);

		ENGINE_INFO_D("Client creating simulation with gravity: ({}, {})", gravityX, gravityY);
	}

	// Push layer to layer stack. Pop layer from layer stack. This will change to being a scene member function.
	// The scene will own the layer stack.
	void Scene::PushLayer(Layer* layer)
	{
		if (layer == nullptr)
		{
			ENGINE_CRITICAL_D("Layer is nullptr! Cannot push to layer stack.");
			return;
		}

		m_layerStack.pushLayer(layer);
		layer->IsAttachedToScene = true;
	}
	void Scene::PopLayer(Layer* layer)
	{
		if (layer == nullptr)
		{
			ENGINE_CRITICAL_D("Layer is nullptr! Cannot pop from layer stack.");
			return;
		}

		m_layerStack.pop(layer);
	}
	void Scene::PopLayer()
	{
		if (m_layerStack.size() == 0)
		{
			ENGINE_CRITICAL_D("Layer stack is empty! Cannot pop from layer stack.");
			return;
		}

		m_layerStack.pop();
	}
}