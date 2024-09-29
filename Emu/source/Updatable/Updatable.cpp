#pragma once

#include "../../include/ComponentManager/Updatable.h"

#include "../../include/Logging/Logger.h"

namespace Engine
{
	Updatable::Updatable(size_t id, UpdateCallback callback)
		: m_id(id), m_callback(callback)
	{
		ENGINE_INFO_D("In updatable constructor: " + std::to_string(id));
		auto& manager = ComponentManagerRegistry::GetManager<Updatable>();
		manager.AddComponent(id, this);
	}

	void Updatable::Update()
	{
		if (m_callback)
		{
			m_callback();
		}
		else
		{
			ENGINE_INFO_D("No Update function");
		}
	}

	Updatable::~Updatable()
	{
		ENGINE_INFO_D("In updatable destructor: " + std::to_string(m_id));
		ComponentManagerRegistry::GetManager<Updatable>().RemoveComponent(m_id);
	}

	size_t Updatable::GetID() const
	{
		return m_id;
	}

}