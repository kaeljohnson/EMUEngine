#pragma once

#include "../include/UpdateSystem.h"

namespace Engine
{
	UpdateSystem::UpdateSystem(ECS& refECS)
		: m_refECS(refECS) {}

	void UpdateSystem::Update()
	{
		// Update all updatable components
		for (auto& refUpdatable : m_refECS.GetHotComponents<Updatable>())
		{
			refUpdatable.Update();
		}
	}
} // namespace Engine