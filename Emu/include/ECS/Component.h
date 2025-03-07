#pragma once

#include "Entity.h"

namespace Engine
{
	class Component
	{
	public:
		Component(Entity* ptrEntity) : m_ptrEntity(ptrEntity) {}
		virtual ~Component() = default;

		inline Entity* GetEntity() const { return m_ptrEntity; }

	private:
		Entity* m_ptrEntity;
	};
}