#pragma once

#include <string>

#include "SceneObject.h"

namespace Engine
{
	// Event Listener Stack.

	class SceneObjectStack
	{
	public:
		static const size_t MAX_OBJECTS = 100;
	private:
		size_t m_sceneObjectCount = 0;
		SceneObject* m_sceneObjects[MAX_OBJECTS];

	public:
		SceneObjectStack();
		~SceneObjectStack() = default;

		SceneObject** const begin() { return m_sceneObjects; }
		SceneObject** const end() { return m_sceneObjects + m_sceneObjectCount; }

		const size_t size() const;

		// Pushes an object to the stack.
		void push(SceneObject* object);

		// Pops an object from the stack.
		void pop(SceneObject* object);

		// Pops the top object from the stack.
		void pop();
	};
}