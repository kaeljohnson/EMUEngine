#pragma once

#include "../../include/Scenes/SceneObjectStack.h"
#include "../../include/Scenes/SceneObjectStack.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	SceneObjectStack::SceneObjectStack()
	{
		for (int i = 0; i < MAX_OBJECTS; i++)
		{
			m_sceneObjects[i] = nullptr;
		}
	}

	const size_t SceneObjectStack::size() const { return m_sceneObjectCount; }

	void SceneObjectStack::push(SceneObject* sceneObject)
	{
		if (sceneObject == nullptr)
		{
			ENGINE_CRITICAL_D("SceneObject is nullptr. Cannot push nullptr to the SceneObject stack.");
			return;
		}

		// Check if the scene object already exists in the array
		for (auto& existingSceneObject : m_sceneObjects)
		{
			if (existingSceneObject == sceneObject)
			{
				ENGINE_WARN_D("SceneObject already exists in scene.");
				return;
			}
		}

		// Check if there's room in the array
		if (m_sceneObjectCount >= MAX_OBJECTS)
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
	}

	void SceneObjectStack::pop(SceneObject* sceneObject)
	{
		if (sceneObject == nullptr)
		{
			ENGINE_CRITICAL_D("SceneObject is nullptr. Cannot pop nullptr from the SceneObject stack.");
			return;
		}

		// Find the object in the array
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
			ENGINE_WARN_D("Scene object not found in scene.");
			return;
		}

		// Remove the scene object from the array
		for (size_t i = index; i < m_sceneObjectCount - 1; i++)
		{
			m_sceneObjects[i] = m_sceneObjects[i + 1];
		}

		m_sceneObjectCount--;
	}

	void SceneObjectStack::pop()
	{
		if (m_sceneObjectCount == 0)
		{
			ENGINE_CRITICAL_D("SceneObject stack is empty. Cannot pop SceneObject from an empty SceneObject stack.");
			return;
		}

		// Decrease the count of event listeners
		m_sceneObjectCount--;
	}
}