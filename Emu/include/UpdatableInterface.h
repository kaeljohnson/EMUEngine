#pragma once

#include "../include/UpdateSystem.h"
#include "../include/ECS/ECS.h"
#include "../include/Components.h"

namespace Engine
{
	class UpdatableInterface
	{
	public:
		// EMU_API void AddUpdatable(Entity* ptrEntity);

	public:
		UpdatableInterface(ECS& refECS);
		~UpdatableInterface() = default;
	private:
		ECS& m_refECS;
	};
}