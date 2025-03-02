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
		EMU_API const Vector2D<float> GetPrevPosition(Entity* ptrEntity);

		// Calling this on a transform that is connected to a physics component is meaningless.
		EMU_API void SetPosition(Entity* ptrEntity, const Vector2D<float> position);
		EMU_API const Vector2D<float> GetPosition(Entity* ptrEntity);
		EMU_API void SetDimensions(Entity* ptrEntity, const Vector2D<float> dimensions);
		EMU_API const Vector2D<float> GetDimensions(Entity* ptrEntity);
		EMU_API void SetZIndex(Entity* ptrEntity, const int zIndex);
		EMU_API const int GetZIndex(Entity* ptrEntity);
		EMU_API void SetRotation(Entity* ptrEntity, const float rotation);
		EMU_API const float GetRotation(Entity* ptrEntity);
		EMU_API void SetScale(Entity* ptrEntity, const float scale);
		EMU_API const float GetScale(Entity* ptrEntity);
		EMU_API void SetDirectionFacing(Entity* ptrEntity, const int direction);
		EMU_API const int GetDirectionFacing(Entity* ptrEntity);

	public:
		Transform* GetTransform(Entity* ptrEntity);
		TransformInterface(ECS& refEcs);
	private:
		ECS& m_refECS;
	};
}