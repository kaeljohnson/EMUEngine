#pragma once

#include "ECS/ECS.h"
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
		
		EMU_API const Vector2D<float> GetPrevPosition(Entity entity);
		EMU_API const Vector2D<float> GetPrevPosition(Transform& transform);

		EMU_API void Activate(Entity entity);
		EMU_API void Deactivate(Entity entity);

		// Calling this on a transform that is connected to a physics component is meaningless.
		EMU_API void SetPosition(Entity entity, const Vector2D<float> position);
		EMU_API const Vector2D<float> GetPosition(Entity entity);
		EMU_API void SetDimensions(Entity entity, const Vector2D<float> dimensions);
		EMU_API const Vector2D<float> GetDimensions(Entity entity);
		EMU_API void SetZIndex(Entity entity, const int zIndex);
		EMU_API const int GetZIndex(Entity entity);
		EMU_API void SetRotation(Entity entity, const float rotation);
		EMU_API const float GetRotation(Entity entity);
		EMU_API void SetScale(Entity entity, const float scale);
		EMU_API const float GetScale(Entity entity);
		EMU_API void SetDirectionFacing(Entity entity, const int direction);
		EMU_API const int GetDirectionFacing(Entity entity);

	public:
		Transform* GetTransform(Entity entity);
		TransformInterface(ECS& refEcs);

		void SetPrevPosition(Entity entity, const Vector2D<float> position);
	private:
		ECS& m_refECS;
	};
}