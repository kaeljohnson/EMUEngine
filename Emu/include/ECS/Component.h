#pragma once

namespace Engine
{
	using Entity = size_t;

	class Component
	{
	public:
		Component(Entity entity, const bool active) : m_entity(entity), m_active(active) {}
		virtual ~Component() = default;

		Entity m_entity;
		bool m_active = true;
	};
}