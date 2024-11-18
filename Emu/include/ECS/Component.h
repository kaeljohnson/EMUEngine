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
		inline const bool IsActive() const { return m_active; }
		inline void SetActive(bool active) 
		{
			active ? OnActivate() : OnDeactivate();
			m_active = active; 
		}
		inline virtual void OnDeactivate() {}
		inline virtual void OnActivate() {}

		inline virtual void OnLoad() {}
		inline virtual void OnUnload() {}

	private:
		Entity* m_ptrEntity;
		bool m_active = false;
	};

}