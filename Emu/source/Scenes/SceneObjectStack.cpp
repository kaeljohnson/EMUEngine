#pragma once

#include "../../include/EngineConstants.h"

#include "../../include/Scenes/SceneObjectStack.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	SceneObjectStack::SceneObjectStack()
	{
		for (int i = 0; i < MAX_OBJECTS; i++)
		{
			m_sceneObjectIDs[i] = 0;
		}
	}

	const size_t SceneObjectStack::Size() const { return m_sceneObjectCount; }

	void SceneObjectStack::Push(const size_t id)
	{
		// Check if the entity already exists in the array
		for (size_t& objectID : m_sceneObjectIDs)
		{
			if (objectID == id)
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

		m_sceneObjectIDs[m_sceneObjectCount] = id;

		m_sceneObjectCount++;
	}

	void SceneObjectStack::Pop(const size_t id)
	{
		// Find the object in the array
		size_t index = 0;
		for (; index < m_sceneObjectCount; index++)
		{
			if (m_sceneObjectIDs[index] == id)
			{
				break;
			}
		}

		// If the scene object was not found, return
		if (index == m_sceneObjectCount)
		{
			ENGINE_WARN_D("Scene object not found in layer.");
			return;
		}

		// If there is only one scene object in the array, set it to nullptr and decrement the count.
		// The for loop below won't run if there is only one scene object in the array.
		if (m_sceneObjectCount == 1)
		{
			m_sceneObjectIDs[0] = 0;
			m_sceneObjectCount--;
			return;
		}

		// Remove the scene object from the array
		for (size_t i = index; i < m_sceneObjectCount - 1; i++)
		{
			m_sceneObjectIDs[i] = m_sceneObjectIDs[i + 1];
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

		m_sceneObjectCount--;
	}
}