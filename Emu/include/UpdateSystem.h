#pragma once

#include "../include/UpdateSystem.h"
#include "../include/ECS/ECS.h"
#include "../include/Components.h"

namespace Engine
{
	class UpdateSystem
	{
	public:
		UpdateSystem(ECS& refECS);
		~UpdateSystem() = default;

		void Update();
	private:
		ECS& m_refECS;

		ComponentManager<Updatable>& m_refUpdatableManager;
	};
}