#pragma once

#include "../include/TransformInterface.h"

namespace Engine
{
	void TransformInterface::SetPrevPosition(Entity entity, const Vector2D<float> position)
	{
		Transform* ptrTransform = GetTransform(entity);
		ptrTransform->PrevPosition = position;
	}

	const Vector2D<float> TransformInterface::GetPrevPosition(Entity entity)
	{
		Transform* ptrTransform = GetTransform(entity);
		return ptrTransform->PrevPosition;
	}

	const Vector2D<float> TransformInterface::GetPrevPosition(Transform& transform)
	{
		return transform.PrevPosition;
	}

	void TransformInterface::SetPosition(Entity entity, const Vector2D<float> position)
	{
		Transform* ptrTransform = GetTransform(entity);
		ptrTransform->Position = position;
	}

	const Vector2D<float> TransformInterface::GetPosition(Entity entity)
	{
		Transform* ptrTransform = GetTransform(entity);
		return ptrTransform->Position;
	}

	void TransformInterface::SetDimensions(Entity entity, const Vector2D<float> dimensions)
	{
		Transform* ptrTransform = GetTransform(entity);
		ptrTransform->Dimensions = dimensions;
	}

	const Vector2D<float> TransformInterface::GetDimensions(Entity entity)
	{
		Transform* ptrTransform = GetTransform(entity);
		return ptrTransform->Dimensions;
	}

	void TransformInterface::SetZIndex(Entity entity, const int zIndex)
	{
		Transform* ptrTransform = GetTransform(entity);
		ptrTransform->ZIndex = zIndex;
	}

	const int TransformInterface::GetZIndex(Entity entity)
	{
		Transform* ptrTransform = GetTransform(entity);
		return ptrTransform->ZIndex;
	}

	void TransformInterface::SetRotation(Entity entity, const float rotation)
	{
		Transform* ptrTransform = GetTransform(entity);
		ptrTransform->Rotation = rotation;
	}

	const float TransformInterface::GetRotation(Entity entity)
	{
		Transform* ptrTransform = GetTransform(entity);
		return ptrTransform->Rotation;
	}

	void TransformInterface::SetScale(Entity entity, const float scale)
	{
		Transform* ptrTransform = GetTransform(entity);
		ptrTransform->Scale = scale;
	}

	const float TransformInterface::GetScale(Entity entity)
	{
		Transform* ptrTransform = GetTransform(entity);
		return ptrTransform->Scale;
	}

	void TransformInterface::SetDirectionFacing(Entity entity, const int direction)
	{
		Transform* ptrTransform = GetTransform(entity);
		ptrTransform->DirectionFacing = direction;
	}

	const int TransformInterface::GetDirectionFacing(Entity entity)
	{
		Transform* ptrTransform = GetTransform(entity);
		return ptrTransform->DirectionFacing;
	}

	Transform* TransformInterface::GetTransform(Entity entity)
	{
		return m_refECS.GetComponent<Transform>(entity);
	}

	TransformInterface::TransformInterface(ECS& refEcs) : m_refECS(refEcs)
	{
	}
}