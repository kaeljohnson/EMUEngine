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
	Scene::Scene(std::string name, const float timestep, const int pixelsPerMeter) : m_name(name), m_pixelsPerMeter(pixelsPerMeter), m_timeStep(timestep)
	{
		m_world = CreateWorld(0.0f * m_pixelsPerMeter, 9.8f * m_pixelsPerMeter, m_timeStep, 8, 3);
		
		for (size_t i = 0; i < m_maxSceneObjects; i++)
		{
			m_sceneObjects[i] = nullptr;
		
		}
	}

	Scene::~Scene()
	{
		delete m_world;
		m_world = nullptr;
	}

	void Scene::checkValid()
	{
		(m_world == nullptr) ? ENGINE_CRITICAL_D("World is nullptr.") : ENGINE_INFO_D("World is valid.");
		(m_sceneObjectCount > m_maxSceneObjects) ? ENGINE_CRITICAL_D("Scene object count exceeds max scene objects.") : ENGINE_INFO_D("Scene object count is valid.");
		(m_pixelsPerMeter <= 0) ? ENGINE_CRITICAL_D("Pixels per meter is invalid.") : ENGINE_INFO_D("Pixels per meter is valid.");
		(m_timeStep <= 0) ? ENGINE_CRITICAL_D("Time step is invalid.") : ENGINE_INFO_D("Time step is valid.");
		(m_name.empty()) ? ENGINE_CRITICAL_D("Scene name is empty.") : ENGINE_INFO_D("Scene name is valid.");
	}

	void Scene::update()
	{
		// Faster way to do this? Should only have to update objects
		// prev values if they have changed. In fact, should only update
		// objects that have changed in general

		size_t idx = 0;
		while (idx < m_sceneObjectCount)
		{
			m_sceneObjects[idx]->GetPhysicsBody()->updatePrevX();
			m_sceneObjects[idx]->GetPhysicsBody()->updatePrevY();
			idx++;
		}

		m_world->update();
	};

	void Scene::SetSimulation(const float gravityX, const float gravityY)
	{
		m_world->SetGravity(gravityX * m_pixelsPerMeter, gravityY * m_pixelsPerMeter);
		m_world->SetTimeStep(m_timeStep);

		ENGINE_INFO_D("Client creating simulation with gravity: ({}, {})", gravityX, gravityY);
	}

	void Scene::Add(SceneObject* sceneObject)
	{
		(sceneObject == nullptr) ? ENGINE_WARN_D("SceneObject is nullptr.") : ENGINE_WARN_D("Adding scene object to scene {}.", m_name);

		// Check if the scene object already exists in the array
		for (size_t i = 0; i < m_sceneObjectCount; i++)
		{
			if (m_sceneObjects[i] == sceneObject)
			{
				ENGINE_WARN_D("SceneObject already exists in scene {}.", m_name);
				return;
			}
		}

		// Check if there's room in the array
		if (m_sceneObjectCount >= m_maxSceneObjects)
		{
			ENGINE_WARN_D("Scene is full. Cannot add more scene objects.");
			return;
		}

		// Box2d performs better when it simulates static objects before dynamic objects.
		// Collisions are more accurate when static objects are simulated first.
		// Thus, we need to add static objects to the beginning of the list.

		if (sceneObject->GetPhysicsBody()->getBodyType() == BodyType::STATIC)
		{
			// Find the first dynamic object
			size_t firstDynamicIndex = 0;
			for (; firstDynamicIndex < m_sceneObjectCount; firstDynamicIndex++)
			{
				if (m_sceneObjects[firstDynamicIndex]->GetPhysicsBody()->getBodyType() != BodyType::STATIC)
				{
					break;
				}
			}

			// Shift all dynamic objects to the right
			for (size_t i = m_sceneObjectCount; i > firstDynamicIndex; i--)
			{
				m_sceneObjects[i] = m_sceneObjects[i - 1];
			}

			// Insert the static object before the first dynamic object
			m_sceneObjects[firstDynamicIndex] = sceneObject;
		}
		else
		{
			// Add rest of objects to the end of the list
			m_sceneObjects[m_sceneObjectCount] = sceneObject;
		}

		m_sceneObjectCount++;

		
		m_world->addBox(static_cast<Box*>(sceneObject->GetPhysicsBody()));
	}

	void Scene::Remove(SceneObject* sceneObject)
	{
		(sceneObject == nullptr) ? ENGINE_WARN_D("SceneObject is nullptr.") : ENGINE_WARN_D("Removing scene object from scene {}.", m_name);

		// Find the scene object in the array
		size_t index = 0;
		for (; index < m_sceneObjectCount; index++)
		{
			if (m_sceneObjects[index] == sceneObject)
			{
				break;
			}
		}

		// If the scene object was not found, return
		if (index == m_sceneObjectCount)
		{
			ENGINE_WARN_D("Scene object not found in scene {}.", m_name);
			return;
		}

		// Remove the scene object from the array
		for (size_t i = index; i < m_sceneObjectCount - 1; i++)
		{
			m_sceneObjects[i] = m_sceneObjects[i + 1];
		}

		m_sceneObjectCount--;

		m_world->removeBox(static_cast<Box*>(sceneObject->GetPhysicsBody()));
	}
}