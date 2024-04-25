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

	}

	Scene::~Scene()
	{
		delete m_world;
		m_world = nullptr;
	}

	void Scene::checkValid()
	{
		// Check if the scene is valid. If not, throw an error.
		for (auto& layer : m_layerStack)
		{
			ENGINE_TRACE_D("Layer: {}", layer->GetName());
		}

		ENGINE_INFO_D("Application running!");

		if (m_layerStack.size() > 0)
		{
			ENGINE_TRACE_D("Layer stack size: {}", m_layerStack.size());
		}
		else
		{
			ENGINE_CRITICAL_D("Layer stack is empty! Application must have at least one layers to be valid!");
			ICallbackSystem::GetInstance()->TriggerCallback(Type::EndApplication, nullptr);
		}
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

		m_layerStack.push(layer);
		// Scene objects added to the application world when layer is pushed to application layer stack.
		//AddToWorld(layer);
		layer->IsAttachedToScene = true;
	}

	void Scene::Add(SceneObject* sceneObject, Layer* layer)
	{
		(sceneObject == nullptr) ? ENGINE_WARN_D("SceneObject is nullptr.") : ENGINE_TRACE_D("Adding scene object to scene {}.", m_name);
		(layer == nullptr) ? ENGINE_WARN_D("Layer is nullptr. Scene object will be rendered last.") : ENGINE_TRACE_D("Scene object tied to {}.", layer->GetName());

		if (std::find(m_sceneObjects.begin(), m_sceneObjects.end(), sceneObject) != m_sceneObjects.end())
		{
			ENGINE_WARN_D("SceneObject already exists in scene {}.", m_name);
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
			layer->AddSceneObject(sceneObject);
		}
		else
		{
			// Add rest of objects to the end of the list.
			m_sceneObjects.push_back(sceneObject);
			layer->AddSceneObject(sceneObject);
		}

		// If the layer is already attached to the scenes layer stack, add the object to the world as well.
		// Could be other reasons the client may or may not want to add the object to the world.
		// More conditions may be added later. Like, visible, collidable, etc.
		if (layer->IsAttachedToScene)
		{
			// ptrICallbackSystem->TriggerCallback(Type::AddToWorld, sceneObject);
			m_world->addBox(static_cast<Box*>(sceneObject->GetPhysicsBody()));
		}
	}

	/*void Scene::AddToWorld(Layer* layer)
	{
		ENGINE_TRACE_D("Adding layer {} to the world.", m_name);

		for (SceneObject* sceneObject : *layer)
		{
			m_world->addBox(static_cast<Box*>(sceneObject->GetPhysicsBody()));
		}
	};*/

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