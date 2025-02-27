#pragma once

#include "ECS/ECS.h"
#include "ECS/Entity.h"
#include "Core.h"
#include "MathUtil.h"
#include "Components.h"
namespace Engine
{
	// Global utility transform interface
	class TransformInterface
	{
	public:
		// Transform getter and setter wrappers
		EMU_API void SetPrevPosition(Entity* ptrEntity, const Vector2D<float> position);
		EMU_API void SetPosition(Entity* ptrEntity, const Vector2D<float> position);
		EMU_API void SetVelocity(Entity* ptrEntity, const Vector2D<float> velocity);
		EMU_API void SetDimensions(Entity* ptrEntity, const Vector2D<float> dimensions);

		EMU_API void SetZIndex(Entity* ptrEntity, const int zIndex);
		EMU_API void SetRotation(Entity* ptrEntity, const float rotation);
		EMU_API void SetScale(Entity* ptrEntity, const float scale);
		EMU_API void SetDirectionFacing(Entity* ptrEntity, const int direction);

	public:
		Transform* GetTransform(Entity* ptrEntity);
		TransformInterface(ECS& refEcs);
	private:
		ECS& m_refECS;
	};
}