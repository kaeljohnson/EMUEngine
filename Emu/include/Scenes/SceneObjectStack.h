#pragma once

#include "../EngineConstants.h"

#include "SceneObject.h"

namespace Engine
{
	class SceneObjectStack
	{
	private:
		size_t m_sceneObjectCount = 0;
		SceneObject* m_sceneObjects[MAX_OBJECTS];

	public:
		SceneObjectStack();
		~SceneObjectStack() = default;

		const size_t Size() const;
		void Push(SceneObject* object);
		void Pop(SceneObject* object);
		void Pop();

		SceneObject** const begin() { return m_sceneObjects; }
		SceneObject** const end() { return m_sceneObjects + m_sceneObjectCount; }
	};
}