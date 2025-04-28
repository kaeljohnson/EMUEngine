#pragma once

#include "../include/UpdatableInterface.h"

namespace Engine
{
	UpdatableInterface::UpdatableInterface(ECS& refECS)
		: m_refECS(refECS) {}

	void UpdatableInterface::Activate(Entity entity)
	{
	}

	void UpdatableInterface::Deactivate(Entity entity)
	{
		// Nothing to do.
	}

	/*void UpdatableInterface::AddUpdatable(Entity* ptrEntity)
	{
		m_refECS.AddComponent<Updatable>(ptrEntity);
	}*/
}