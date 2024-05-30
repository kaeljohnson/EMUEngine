#pragma once

#include "../../include/EngineConstants.h"

#include "../../include/Scenes/SceneObjectStack.h"
#include "../../include/Scenes/SceneObject.h"
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

	const size_t SceneObjectStack::Size() const { return m_sceneObjectCount; }

	void SceneObjectStack::Push(SceneObject* sceneObject)
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

		// Add rest of objects to the end of the list
		m_sceneObjects[m_sceneObjectCount] = sceneObject;

		m_sceneObjectCount++;
	}

	void SceneObjectStack::Pop(SceneObject* sceneObject)
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

	void SceneObjectStack::Pop()
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