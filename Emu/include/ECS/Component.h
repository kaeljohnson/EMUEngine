#pragma once

namespace Engine
{
	using Entity = size_t;

	/**
	* @brief Base class for all components in the ECS.
	 *
	 * Each component is associated with an entity.
	 * 
	 * Components represent different attributes or behaviors that an 
	 * entity can encompass.
	*/
	class Component
	{
	public:
		Component(Entity entity) : m_entity(entity) {}
		virtual ~Component() = default;

		/**
		 * @brief Gets the entity associated with this component.
		 */
		Entity m_entity;
	};
}