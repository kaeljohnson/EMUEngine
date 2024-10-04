#pragma once

#include "../../include/ComponentManager/Updatable.h"

#include "../../include/Logging/Logger.h"

namespace Engine
{
	Updatable::Updatable(size_t id, UpdateCallback callback)
		: m_id(id), m_callback(callback)
	{
		// auto& manager = ComponentManagerRegistry::GetManager<Updatable>();
		// manager.AddComponent(id, this);
	}

	void Updatable::Update()
	{
		if (m_callback)
		{
			m_callback();
		}
		else
		{
			ENGINE_INFO_D("No Update function for updatable component: " + std::to_string(m_id));
		}
	}

	Updatable::~Updatable()
	{
		ENGINE_INFO_D("In updatable destructor: " + std::to_string(m_id));
		// ComponentManagerRegistry::GetManager<Updatable>().RemoveComponent(m_id);
	}
}