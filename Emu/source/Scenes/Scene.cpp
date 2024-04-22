#pragma once

#include <string>

#include "../../include/Scenes/Scene.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Physics/Box.h"

namespace Engine
{
	Scene::Scene(std::string name) : m_name(name)
	{

	}

	Scene::~Scene() = default;

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

}