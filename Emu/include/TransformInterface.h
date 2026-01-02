#pragma once

#include "ECS/ECS.h"
#include "MathUtil.h"
#include "Components.h"
namespace Engine
{
	// Global utility transform interface
	class TransformInterface
	{
	public:
		// Transform getter and setter wrappers
		const Math2D::Point2D<float> GetPrevPosition(Entity entity);
		const Math2D::Point2D<float> GetPrevPosition(Transform& transform);

		// Calling this on a transform that is connected to a physics component is meaningless.
		void SetPosition(Entity entity, const Math2D::Point2D<float> position);
		const Math2D::Point2D<float> GetPosition(Entity entity);
		void SetZIndex(Entity entity, const int zIndex);
		const size_t GetZIndex(Entity entity);
		void SetRotation(Entity entity, const float rotation);
		const float GetRotation(Entity entity);
		void SetScale(Entity entity, const float scale);
		const float GetScale(Entity entity);
		void SetDirectionFacing(Entity entity, const int direction);
		const int GetDirectionFacing(Entity entity);

	public:
		Transform* GetTransform(Entity entity);
		TransformInterface(ECS& refEcs);

		void SetPrevPosition(Entity entity, const Math2D::Point2D<float> position);
	private:
		ECS& m_refECS;
	};
}