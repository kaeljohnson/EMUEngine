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
		* @brief Constructs the TransformInterface with a reference to the ECS.
		* 
		* @param refEcs Reference to the ECS instance.
		*/
		TransformInterface(ECS& refEcs);

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
		* @brief Get the position of the transform on the screen. This is used for rendering and is calculated by the camera system.
		* 
		* @param entity The entity whose transform to get the screen position of.
		* 
		* @return The screen position of the entity's transform.
		*/
		const Math2D::Vector2D<float> GetScreenPosition(Entity entity) { return { GetTransform(entity)->m_screenTopLeft, GetTransform(entity)->m_screenBottomRight }; }

		/** 
		* @brief Set position of transform.
		* 
		* @Note Calling this on an entity with a physics body does nothing as physics system controls position.
		* 
		* @param entity The entity whose transform to set the position of.
		* @param position The new position of the entity's transform in world space.
		* @param skipLerp If true, the transform will immediately jump to the new position without interpolation. If false, the transform will smoothly interpolate to the new position over time.
		*/
		void SetPosition(Entity entity, const Math2D::Point2D<float> position, bool skipLerp);

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

	private:
		ECS& m_refECS;
	};
}