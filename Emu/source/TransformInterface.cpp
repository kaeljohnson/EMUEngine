#pragma once

#include "../include/TransformInterface.h"

namespace Engine
{
	void TransformInterface::SetPrevPosition(Entity* ptrEntity, const Vector2D<float> position)
	{
		Transform* ptrTransform = GetTransform(ptrEntity);
		ptrTransform->PrevPosition = position;
	}

	void TransformInterface::SetPosition(Entity* ptrEntity, const Vector2D<float> position)
	{
		Transform* ptrTransform = GetTransform(ptrEntity);
		ptrTransform->Position = position;
	}

	void TransformInterface::SetVelocity(Entity* ptrEntity, const Vector2D<float> velocity)
	{
		Transform* ptrTransform = GetTransform(ptrEntity);
		ptrTransform->Velocity = velocity;
	}

	void TransformInterface::SetDimensions(Entity* ptrEntity, const Vector2D<float> dimensions)
	{
		Transform* ptrTransform = GetTransform(ptrEntity);
		ptrTransform->Dimensions = dimensions;
	}

	void TransformInterface::SetZIndex(Entity* ptrEntity, const int zIndex)
	{
		Transform* ptrTransform = GetTransform(ptrEntity);
		ptrTransform->ZIndex = zIndex;
	}

	void TransformInterface::SetRotation(Entity* ptrEntity, const float rotation)
	{
		Transform* ptrTransform = GetTransform(ptrEntity);
		ptrTransform->Rotation = rotation;
	}

	void TransformInterface::SetScale(Entity* ptrEntity, const float scale)
	{
		Transform* ptrTransform = GetTransform(ptrEntity);
		ptrTransform->Scale = scale;
	}

	void TransformInterface::SetDirectionFacing(Entity* ptrEntity, const int direction)
	{
		Transform* ptrTransform = GetTransform(ptrEntity);
		ptrTransform->DirectionFacing = direction;
	}

	Transform* TransformInterface::GetTransform(Entity* ptrEntity)
	{
		return m_refECS.GetComponent<Transform>(ptrEntity);
	}

	TransformInterface::TransformInterface(ECS& refEcs) : m_refECS(refEcs)
	{
	}
}