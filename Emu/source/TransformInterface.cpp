#pragma once

#include "../include/TransformInterface.h"

namespace Engine
{
	void TransformInterface::SetPrevPosition(Entity entity, const Math2D::Point2D<float> position)
	{
		Transform* ptrTransform = GetTransform(entity);
		ptrTransform->m_prevPosition = position;
	}

	const Math2D::Point2D<float> TransformInterface::GetPrevPosition(Entity entity)
	{
		Transform* ptrTransform = GetTransform(entity);
		return ptrTransform->m_prevPosition;
	}

	const Math2D::Point2D<float> TransformInterface::GetPrevPosition(Transform& transform)
	{
		return transform.m_prevPosition;
	}

	void TransformInterface::SetPosition(Entity entity, const Math2D::Point2D<float> position)
	{
		Transform* ptrTransform = GetTransform(entity);
		ptrTransform->m_position = position;
	}

	const Math2D::Point2D<float> TransformInterface::GetPosition(Entity entity)
	{
		Transform* ptrTransform = GetTransform(entity);
		return ptrTransform->m_position;
	}

	void TransformInterface::SetDimensions(Entity entity, const Math2D::Point2D<float> dimensions)
	{
		Transform* ptrTransform = GetTransform(entity);
		ptrTransform->m_dimensions = dimensions;
	}

	const Math2D::Point2D<float> TransformInterface::GetDimensions(Entity entity)
	{
		Transform* ptrTransform = GetTransform(entity);
		return ptrTransform->m_dimensions;
	}

	void TransformInterface::SetZIndex(Entity entity, const int zIndex)
	{
		Transform* ptrTransform = GetTransform(entity);
		ptrTransform->m_zIndex = zIndex;
	}

	const size_t TransformInterface::GetZIndex(Entity entity)
	{
		Transform* ptrTransform = GetTransform(entity);
		return ptrTransform->m_zIndex;
	}

	void TransformInterface::SetRotation(Entity entity, const float rotation)
	{
		Transform* ptrTransform = GetTransform(entity);
		ptrTransform->m_rotation = rotation;
	}

	const float TransformInterface::GetRotation(Entity entity)
	{
		Transform* ptrTransform = GetTransform(entity);
		return ptrTransform->m_rotation;
	}

	void TransformInterface::SetScale(Entity entity, const float scale)
	{
		Transform* ptrTransform = GetTransform(entity);
		ptrTransform->m_scale = scale;
	}

	const float TransformInterface::GetScale(Entity entity)
	{
		Transform* ptrTransform = GetTransform(entity);
		return ptrTransform->m_scale;
	}

	void TransformInterface::SetDirectionFacing(Entity entity, const int direction)
	{
		Transform* ptrTransform = GetTransform(entity);
		ptrTransform->m_directionFacing = direction;
	}

	const int TransformInterface::GetDirectionFacing(Entity entity)
	{
		Transform* ptrTransform = GetTransform(entity);
		return ptrTransform->m_directionFacing;
	}

	Transform* TransformInterface::GetTransform(Entity entity)
	{
		return m_refECS.GetComponent<Transform>(entity);
	}

	TransformInterface::TransformInterface(ECS& refEcs) : m_refECS(refEcs)
	{
	}
}