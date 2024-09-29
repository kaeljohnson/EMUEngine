#pragma once

#include "../EngineConstants.h"

namespace Engine
{
	class SceneObjectStack
	{
	private:
		size_t m_sceneObjectCount = 0;
		size_t m_sceneObjectIDs[MAX_OBJECTS];

	public:
		SceneObjectStack();
		~SceneObjectStack() = default;

		const size_t Size() const;
		void Push(const size_t id);
		void Pop(const size_t object);
		void Pop();

		const size_t* const begin() { return m_sceneObjectIDs; }
		const size_t* const end() { return m_sceneObjectIDs + m_sceneObjectCount; }
	};
}