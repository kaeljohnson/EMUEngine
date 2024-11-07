#pragma once

#include "Entity.h"

namespace Engine
{
	class Component
	{
	public:
		Component(Entity& refEntity) : m_entity(&refEntity) {}
		virtual ~Component() = default;

		inline const Entity* GetEntity() const { return m_entity; }
		inline const bool IsActive() const { return m_active; }
		inline void SetActive(bool active) { m_active = active; }

	private:
		Entity* m_entity;
		bool m_active = false;
	};

}