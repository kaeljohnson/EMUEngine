#pragma once

namespace Engine
{
	using EntityID = size_t;

	struct Entity
	{
	public:
		Entity(EntityID id) : ID(id) {}
		EntityID GetID() const { return ID; }

	private:
		EntityID ID;
	};
}