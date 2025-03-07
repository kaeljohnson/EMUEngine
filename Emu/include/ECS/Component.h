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
		
		inline virtual void OnDeactivate() {}
		inline virtual void OnActivate() {}

	private:
		Entity* m_ptrEntity;
		// bool m_active = false;
	};

}