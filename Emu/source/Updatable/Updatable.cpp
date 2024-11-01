#pragma once

#include "../../include/Updatable/Updatable.h"

#include "../../include/Logging/Logger.h"

namespace Engine
{
	Updatable::Updatable(size_t id, UpdateCallback callback)
		: m_callback(callback), Component(id)
	{
		ENGINE_CRITICAL_D("Updatable constructor called with ID: " + std::to_string(m_id));
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

	Updatable::~Updatable() {}
}