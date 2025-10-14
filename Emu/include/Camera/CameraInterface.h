#pragma once

#include "../Core.h"
#include "../MathUtil.h"
#include "../ECS/ECS.h"

namespace Engine
{
	class CameraInterface
	{
	public:
		void SetPixelsPerUnit(Entity entity, const int pixelsPerUnit);
		const int GetPixelsPerUnit(Entity entity);
		void SetOffset(Entity entity, const Vector2D<float> offset);
		const Vector2D<float> GetOffset(Entity entity);
		void SetClampingOn(Entity entity, const bool clampingOn);
		const bool GetClampingOn(Entity entity);
		const Vector2D<float> GetSize(Entity entity);
		void SetOffsets(Entity entity, const Vector2D<float> offsets);

		CameraInterface(ECS& refECS);
		~CameraInterface() = default;

	private:
		ECS& m_refECS;
	};
}