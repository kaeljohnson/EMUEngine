#pragma once

#include "Entity.h"

namespace Engine
{
	class Component
	{
	public:
		Component(Entity* ptrEntity) : m_ptrEntity(ptrEntity) {}
		virtual ~Component() = default;

		inline const Entity* GetEntity() const { return m_ptrEntity; }
		inline const bool IsActive() const { return m_active; }
		inline void SetActive(bool active) { m_active = active; }

	private:
		Entity* m_ptrEntity;
		bool m_active = false;
	};

}