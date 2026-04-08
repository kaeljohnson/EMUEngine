#pragma once

#include "../include/TransformInterface.h"

namespace Engine
{
	const Math2D::Point2D<float> TransformInterface::GetPrevPosition(Entity entity)
	{
		Transform* ptrTransform = GetTransform(entity);
		return ptrTransform->m_prevPosition;
	}

	const Math2D::Point2D<float> TransformInterface::GetPrevPosition(Transform& transform)
	{
		return transform.m_prevPosition;
	}

	void TransformInterface::SetPosition(Entity entity, const Math2D::Point2D<float> position, bool skipLerp)
	{
		Transform* ptrTransform = GetTransform(entity);
		ptrTransform->m_prevPosition = ptrTransform->m_position;
		ptrTransform->m_position = position;
		ptrTransform->m_skipLerp = skipLerp;
	}

	const Math2D::Point2D<float> TransformInterface::GetPosition(Entity entity)
	{
		Transform* ptrTransform = GetTransform(entity);
		return ptrTransform->m_position;
	}

	void TransformInterface::SetLayer(Entity entity, const int layer)
	{
		Transform* ptrTransform = GetTransform(entity);
		ptrTransform->m_layer = layer;
	}

	const size_t TransformInterface::GetLayer(Entity entity)
	{
		Transform* ptrTransform = GetTransform(entity);
		return ptrTransform->m_layer;
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

	const Math2D::Point2D<float> TransformInterface::GetRenderPosition(Entity entity) 
	{
		return GetTransform(entity)->m_renderPosition;
	}

	TransformInterface::TransformInterface(ECS& refEcs) : m_refECS(refEcs)
	{
	}
}