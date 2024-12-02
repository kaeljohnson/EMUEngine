#pragma once

#include "../../include/Components/Updatable.h"

#include "../../include/Logging/Logger.h"

namespace Engine
{
	Updatable::Updatable(Entity* entity, UpdateCallback callback)
		: m_callback(callback), Component(entity)
	{
	}

	void Updatable::Update()
	{
		if (m_callback)
		{
			m_callback();
		}
	}

	Updatable::~Updatable() {}
}