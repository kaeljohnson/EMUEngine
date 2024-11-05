#pragma once

using EntityID = size_t;

struct Entity
{
	EntityID ID;
	size_t Priority;

	Entity(EntityID id, size_t priority) : ID(id), Priority(priority) {}
};