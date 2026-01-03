#pragma once

#include "../MathUtil.h"
#include "../ECS/ECS.h"

namespace Engine
{
	/**
	* @brief Camera interface.
	*
	* Provides methods to interact with camera components in the ECS.
	*/
	class CameraInterface
	{
	public:
		/**
		* @brief Sets the pixels per unit for the entity's camera. A smaller pixels per unit
		* value results in a more zoomed-out view.
		* 
		* * @param entity The entity whose camera component to modify.
		* * @param pixelsPerUnit The new pixels per unit value.
		*/
		void SetPixelsPerUnit(Entity entity, const int pixelsPerUnit);

		/**
		* @brief Gets the pixels per unit for the entity's camera.
		* 
		* * @param entity The entity whose camera component to query.
		* * @return The current pixels per unit value.
		*/
		const size_t GetPixelsPerUnit(Entity entity);

		/**
		* @brief Sets the offset for the entity's camera. The offset shifts the camera's view
		* relative to the entity's position.
		* 
		* * @param entity The entity whose camera component to modify.
		* * @param offset The new offset value.
		*/
		void SetOffset(Entity entity, const Math2D::Point2D<float> offset);

		/**
		* @brief Gets the offset for the entity's camera.
		* 
		* * @param entity The entity whose camera component to query.
		* * @return The current offset value.
		*/
		const Math2D::Point2D<float> GetOffset(Entity entity);

		/**
		* @brief Sets whether clamping is enabled for the entity's camera.
		* 
		* * @param entity The entity whose camera component to modify.
		* * @param clampingOn True to enable clamping, false to disable.
		*/
		void SetClampingOn(Entity entity, const bool clampingOn);

		/**
		* @brief Gets whether clamping is enabled for the entity's camera.
		* Clamping restricts the camera's movement within defined bounds.
		* For now, its the map bounds.
		* 
		* * @param entity The entity whose camera component to query.
		* * @return True if clamping is enabled, false otherwise.
		*/
		const bool GetClampingOn(Entity entity);

		/**
		* @brief Gets the size for the entity's camera.
		* 
		* * @param entity The entity whose camera component to query.
		* * @return The current size value.
		*/
		const Math2D::Point2D<float> GetSize(Entity entity);

		CameraInterface(ECS& refECS);
		~CameraInterface() = default;

	private:
		ECS& m_refECS;
	};
}