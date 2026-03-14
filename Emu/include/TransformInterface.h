#pragma once

#include "ECS/ECS.h"
#include "../Public/MathUtil.h"
#include "Components.h"
namespace Engine
{
	/**
	* @class TransformInterface
	*
	* @brief Global utility transform interface
	*/
	class TransformInterface
	{
	public:
		/** 
		* @brief Get previous position of an entity's transform in world space.
		* 
		* @param entity The entity whose transform to get the previous position of.
		* 
		* @return The previous position of the entity's transform in world space.
		*/
		const Math2D::Point2D<float> GetPrevPosition(Entity entity);

		/**
		* @brief Get previous position of a transform in world space.
		* 
		* @param transform The transform to get the previous position of.
		* 
		* @return The previous position of the transform in world space.
		*/
		const Math2D::Point2D<float> GetPrevPosition(Transform& transform);

		/** 
		* @brief Set position of transform.
		* 
		* @Note Calling this on an entity with a physics body does nothing as physics system controls position.
		* 
		* @param entity The entity whose transform to set the position of.
		* @param position The new position of the entity's transform in world space.
		*/
		void SetPosition(Entity entity, const Math2D::Point2D<float> position);

		/**
		* @brief Get position of an entity's transform in world space.
		* 
		* @param entity The entity whose transform to get the position of.
		* 
		* @return The position of the entity's transform in world space.
		*/
		const Math2D::Point2D<float> GetPosition(Entity entity);

		/**
		* @brief Set layer of an entity's transform.
		* 
		* @param entity The entity whose transform to set the layer of.
		* @param layer The new layer of the entity's transform.
		*/
		void SetLayer(Entity entity, const int layer);

		/**
		* @brief Get layer of an entity's transform.
		* 
		* @param entity The entity whose transform to get the layer of.
		* 
		* @return The layer of the entity's transform.
		*/
		const size_t GetLayer(Entity entity);

		/**
		* @brief Set rotation of an entity's transform.
		* 
		* @param entity The entity whose transform to set the rotation of.
		* @param rotation The new rotation of the entity's transform in degrees.
		*/
		void SetRotation(Entity entity, const float rotation);

		/**
		* @brief Get rotation of an entity's transform.
		* 
		* @param entity The entity whose transform to get the rotation of.
		* 
		* @return The rotation of the entity's transform in degrees.
		*/
		const float GetRotation(Entity entity);

		/**
		* @brief Set direction facing of an entity's transform.
		* 
		* @param entity The entity whose transform to set the direction facing of.
		* @param direction The new direction facing of the entity's transform. (1 = right, -1 = left)
		*/
		void SetDirectionFacing(Entity entity, const int direction);

		/**
		* @brief Get direction facing of an entity's transform.
		* 
		* @param entity The entity whose transform to get the direction facing of.
		* 
		* @return The direction facing of the entity's transform. (1 = right, -1 = left)
		*/
		const int GetDirectionFacing(Entity entity);

		/**
		* @brief Get transform component of an entity.
		* 
		* @param entity The entity whose transform component to get.
		* 
		* @return Pointer to the entity's transform component.
		*/
		Transform* GetTransform(Entity entity);

		/**
		* @brief Constructor for TransformInterface.
		* 
		* @param refEcs Reference to the ECS instance.
		*/
		TransformInterface(ECS& refEcs);

	private:
		ECS& m_refECS;
	};
}