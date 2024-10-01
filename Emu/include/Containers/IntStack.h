#pragma once

#include "../EngineConstants.h"

namespace Engine
{
	class IntStack
	{
	private:
		int m_objectCount = 0;
		int m_ints[MAX_OBJECTS];

	public:
		IntStack();
		~IntStack() = default;

		const int Size() const;
		void Push(const int id);
		void Pop(const int object);
		void Pop();

		const int* const begin() { return m_ints; }
		const int* const end() { return m_ints + m_objectCount; }
	};
}