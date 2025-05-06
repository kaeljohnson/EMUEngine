#pragma once

#include "../Core.h"
#include "../MathUtil.h"
#include "../ECS/ECS.h"
#include "../Components.h"

namespace Engine
{
	// Any camera component functions that have to do with setting camera values should be in this class.
	class CameraInterface
	{
	public:
		// EMU_API void ChangeCamera(Entity entity);
		EMU_API void SetPixelsPerUnit(Entity entity, const int pixelsPerUnit);
		EMU_API const int GetPixelsPerUnit(Entity entity);
		EMU_API void SetOffset(Entity entity, const Vector2D<float> offset);
		EMU_API const Vector2D<float> GetOffset(Entity entity);
		EMU_API void SetClampingOn(Entity entity, const bool clampingOn);
		EMU_API const bool GetClampingOn(Entity entity);
		EMU_API void SetSize(Entity entity);
		EMU_API const Vector2D<float> GetSize(Entity entity);
		EMU_API void SetOffsets(Entity entity, const Vector2D<float> offsets);

		virtual ~CameraInterface() = default;
	
	public:
		CameraInterface(ECS& refECS);
		Camera* GetCamera(Entity entity);

	private:
		ECS& m_refECS;
	};
}