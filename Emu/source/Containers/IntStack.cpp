#pragma once

#include "../../include/EngineConstants.h"

#include "../../include/Containers/IntStack.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	IntStack::IntStack()
	{
		for (int i = 0; i < MAX_OBJECTS; i++)
		{
			m_ints[i] = -1;
		}
	}

	const int IntStack::Size() const { return m_objectCount; }

	void IntStack::Push(const int num)
	{
		// Check if the entity already exists in the array
		for (int& integer : m_ints)
		{
			if (integer == num)
			{
				ENGINE_WARN_D("SceneObject already exists in scene.");
				return;
			}
		}

		// Check if there's room in the array
		if (m_objectCount >= MAX_OBJECTS)
		{
			ENGINE_WARN_D("Scene is full. Cannot add more scene objects.");
			return;
		}

		m_ints[m_objectCount] = num;

		m_objectCount++;
	}

	void IntStack::Pop(const int id)
	{
		// Find the object in the array
		size_t index = 0;
		for (; index < m_objectCount; index++)
		{
			if (m_ints[index] == id)
			{
				break;
			}
		}

		// If the scene object was not found, return
		if (index == m_objectCount)
		{
			ENGINE_WARN_D("Scene object not found in layer.");
			return;
		}

		// If there is only one scene object in the array, set it to nullptr and decrement the count.
		// The for loop below won't run if there is only one scene object in the array.
		if (m_objectCount == 1)
		{
			m_ints[0] = 0;
			m_objectCount--;
			return;
		}

		// Remove the scene object from the array
		for (size_t i = index; i < m_objectCount - 1; i++)
		{
			m_ints[i] = m_ints[i + 1];
		}

		m_objectCount--;
	}

	void IntStack::Pop()
	{
		if (m_objectCount == 0)
		{
			ENGINE_CRITICAL_D("SceneObject stack is empty. Cannot pop SceneObject from an empty SceneObject stack.");
			return;
		}

		m_objectCount--;
	}
}