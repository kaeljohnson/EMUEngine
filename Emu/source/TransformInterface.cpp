#pragma once

#include "../include/TransformInterface.h"

namespace Engine
{
	void TransformInterface::Activate(Entity entity)
	{
		// Nothing to do.
	}

	void TransformInterface::Deactivate(Entity entity)
	{
		// Nothing to do.
	}

	void TransformInterface::SetPrevPosition(Entity entity, const Vector2D<float> position)
	{
		Transform* ptrTransform = GetTransform(entity);
		ptrTransform->PrevPosition = position;
	}

	void TransformInterface::SetPrevPosition(Transform& transform, const Vector2D<float> position)
	{
		transform.PrevPosition = position;
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

	void TransformInterface::SetPosition(Transform& transform, const Vector2D<float> position)
	{
		transform.Position = position;
	}

	const Vector2D<float> TransformInterface::GetPosition(Entity entity)
	{
		Transform* ptrTransform = GetTransform(entity);
		return ptrTransform->Position;
	}

	const Vector2D<float> TransformInterface::GetPosition(Transform& transform)
	{
		return transform.Position;
	}

	void TransformInterface::SetDimensions(Entity entity, const Vector2D<float> dimensions)
	{
		Transform* ptrTransform = GetTransform(entity);
		ptrTransform->Dimensions = dimensions;
	}

	void TransformInterface::SetDimensions(Transform& transform, const Vector2D<float> dimensions)
	{
		transform.Dimensions = dimensions;
	}

	const Vector2D<float> TransformInterface::GetDimensions(Entity entity)
	{
		Transform* ptrTransform = GetTransform(entity);
		return ptrTransform->Dimensions;
	}

	const Vector2D<float> TransformInterface::GetDimensions(Transform& transform)
	{
		return transform.Dimensions;
	}

	void TransformInterface::SetZIndex(Entity entity, const int zIndex)
	{
		Transform* ptrTransform = GetTransform(entity);
		ptrTransform->ZIndex = zIndex;
	}

	void TransformInterface::SetZIndex(Transform& transform, const int zIndex)
	{
		transform.ZIndex = zIndex;
	}

	const int TransformInterface::GetZIndex(Entity entity)
	{
		Transform* ptrTransform = GetTransform(entity);
		return ptrTransform->ZIndex;
	}

	const int TransformInterface::GetZIndex(Transform& transform)
	{
		return transform.ZIndex;
	}

	void TransformInterface::SetRotation(Entity entity, const float rotation)
	{
		Transform* ptrTransform = GetTransform(entity);
		ptrTransform->Rotation = rotation;
	}

	void TransformInterface::SetRotation(Transform& transform, const float rotation)
	{
		transform.Rotation = rotation;
	}

	const float TransformInterface::GetRotation(Entity entity)
	{
		Transform* ptrTransform = GetTransform(entity);
		return ptrTransform->Rotation;
	}

	const float TransformInterface::GetRotation(Transform& transform)
	{
		return transform.Rotation;
	}

	void TransformInterface::SetScale(Entity entity, const float scale)
	{
		Transform* ptrTransform = GetTransform(entity);
		ptrTransform->Scale = scale;
	}

	void TransformInterface::SetScale(Transform& transform, const float scale)
	{
		transform.Scale = scale;
	}

	const float TransformInterface::GetScale(Entity entity)
	{
		Transform* ptrTransform = GetTransform(entity);
		return ptrTransform->Scale;
	}

	const float TransformInterface::GetScale(Transform& transform)
	{
		return transform.Scale;
	}

	void TransformInterface::SetDirectionFacing(Entity entity, const int direction)
	{
		Transform* ptrTransform = GetTransform(entity);
		ptrTransform->DirectionFacing = direction;
	}

	void TransformInterface::SetDirectionFacing(Transform& transform, const int direction)
	{
		transform.DirectionFacing = direction;
	}

	const int TransformInterface::GetDirectionFacing(Entity entity)
	{
		Transform* ptrTransform = GetTransform(entity);
		return ptrTransform->DirectionFacing;
	}

	const int TransformInterface::GetDirectionFacing(Transform& transform)
	{
		return transform.DirectionFacing;
	}

	Transform* TransformInterface::GetTransform(Entity entity)
	{
		return m_refECS.GetComponent<Transform>(entity);
	}

	TransformInterface::TransformInterface(ECS& refEcs) : m_refECS(refEcs)
	{
	}
}