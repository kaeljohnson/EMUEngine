#pragma once

#include "../../include/ECS/ECS.h"

namespace Engine
{
	size_t ECS::maxID = 0;
	std::unordered_set<size_t> ECS::usedIDs;
	// std::unordered_map<EntityID, Entity*> ECS::m_entities;
	std::unordered_map<std::type_index, std::unique_ptr<Engine::ComponentManagerBase>> ECS::m_componentManagers;
}