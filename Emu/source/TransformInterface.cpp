#pragma once

#include "../include/TransformInterface.h"

namespace Engine
{
	void TransformInterface::Activate(Entity* ptrEntity)
	{
		// Nothing to do.
	}

	void TransformInterface::Deactivate(Entity* ptrEntity)
	{
		// Nothing to do.
	}

	void TransformInterface::SetPrevPosition(Entity* ptrEntity, const Vector2D<float> position)
	{
		Transform* ptrTransform = GetTransform(ptrEntity);
		ptrTransform->PrevPosition = position;
	}

	const Vector2D<float> TransformInterface::GetPrevPosition(Entity* ptrEntity)
	{
		Transform* ptrTransform = GetTransform(ptrEntity);
		return ptrTransform->PrevPosition;
	}

	void TransformInterface::SetPosition(Entity* ptrEntity, const Vector2D<float> position)
	{
		Transform* ptrTransform = GetTransform(ptrEntity);
		ptrTransform->Position = position;
	}

	const Vector2D<float> TransformInterface::GetPosition(Entity* ptrEntity)
	{
		Transform* ptrTransform = GetTransform(ptrEntity);
		return ptrTransform->Position;
	}

	void TransformInterface::SetDimensions(Entity* ptrEntity, const Vector2D<float> dimensions)
	{
		Transform* ptrTransform = GetTransform(ptrEntity);
		ptrTransform->Dimensions = dimensions;
	}

	const Vector2D<float> TransformInterface::GetDimensions(Entity* ptrEntity)
	{
		Transform* ptrTransform = GetTransform(ptrEntity);
		return ptrTransform->Dimensions;
	}

	void TransformInterface::SetZIndex(Entity* ptrEntity, const int zIndex)
	{
		Transform* ptrTransform = GetTransform(ptrEntity);
		ptrTransform->ZIndex = zIndex;
	}

	const int TransformInterface::GetZIndex(Entity* ptrEntity)
	{
		Transform* ptrTransform = GetTransform(ptrEntity);
		return ptrTransform->ZIndex;
	}

	void TransformInterface::SetRotation(Entity* ptrEntity, const float rotation)
	{
		Transform* ptrTransform = GetTransform(ptrEntity);
		ptrTransform->Rotation = rotation;
	}

	const float TransformInterface::GetRotation(Entity* ptrEntity)
	{
		Transform* ptrTransform = GetTransform(ptrEntity);
		return ptrTransform->Rotation;
	}

	void TransformInterface::SetScale(Entity* ptrEntity, const float scale)
	{
		Transform* ptrTransform = GetTransform(ptrEntity);
		ptrTransform->Scale = scale;
	}

	const float TransformInterface::GetScale(Entity* ptrEntity)
	{
		Transform* ptrTransform = GetTransform(ptrEntity);
		return ptrTransform->Scale;
	}

	void TransformInterface::SetDirectionFacing(Entity* ptrEntity, const int direction)
	{
		Transform* ptrTransform = GetTransform(ptrEntity);
		ptrTransform->DirectionFacing = direction;
	}

	const int TransformInterface::GetDirectionFacing(Entity* ptrEntity)
	{
		Transform* ptrTransform = GetTransform(ptrEntity);
		return ptrTransform->DirectionFacing;
	}

	Transform* TransformInterface::GetTransform(Entity* ptrEntity)
	{
		return m_refECS.GetComponent<Transform>(ptrEntity);
	}

	TransformInterface::TransformInterface(ECS& refEcs) : m_refECS(refEcs)
	{
	}
}