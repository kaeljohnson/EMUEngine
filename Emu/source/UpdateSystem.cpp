#pragma once

#include "../include/UpdateSystem.h"

namespace Engine
{
	UpdateSystem::UpdateSystem(ECS& refECS)
		: m_refECS(refECS), m_refUpdatableManager(m_refECS.GetComponentManager<Updatable>()) {}

	void UpdateSystem::Update()
	{
		// Update all updatable components
		for (auto& refUpdatable : m_refUpdatableManager)
		{
			refUpdatable.Update();
		}
	}
} // namespace Engine