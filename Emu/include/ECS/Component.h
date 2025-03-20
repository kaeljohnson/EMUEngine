#pragma once

namespace Engine
{
	using Entity = size_t;

	class Component
	{
	public:
		Component(Entity entity) : m_entity(entity) {}
		virtual ~Component() = default;

		Entity m_entity;
	};
}