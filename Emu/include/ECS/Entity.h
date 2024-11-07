#pragma once

namespace Engine
{
	using EntityID = size_t;

	struct Entity
	{
	public:
		Entity(EntityID id) : ID(id), Priority(0) {}
		void SetPriority(size_t priority) { Priority = priority; }
		size_t GetPriority() const { return Priority; }
		EntityID GetID() const { return ID; }

	private:
		EntityID ID;
		size_t Priority;
	};
}