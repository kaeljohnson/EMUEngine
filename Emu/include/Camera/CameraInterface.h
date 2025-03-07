#pragma once

#include "../Core.h"
#include "../MathUtil.h"
#include "../ECS/ECS.h"
#include "../ECS/Entity.h"
#include "../Components.h"

namespace Engine
{
	// Any camera component functions that have to do with setting camera values should be in this class.
	class CameraInterface
	{
	public:
		EMU_API void Activate(Entity* ptrEntity);
		EMU_API void Deactivate(Entity* ptrEntity);
		EMU_API void SetPixelsPerUnit(Entity* ptrEntity, const int pixelsPerUnit);
		EMU_API const int GetPixelsPerUnit(Entity* ptrEntity);
		EMU_API void SetOffset(Entity* ptrEntity, const Vector2D<float> offset);
		EMU_API const Vector2D<float> GetOffset(Entity* ptrEntity);
		EMU_API void SetClampingOn(Entity* ptrEntity, const bool clampingOn);
		EMU_API const bool GetClampingOn(Entity* ptrEntity);
		EMU_API void SetSize(Entity* ptrEntity);
		EMU_API const Vector2D<float> GetSize(Entity* ptrEntity);
		EMU_API void SetOffsets(Entity* ptrEntity, const Vector2D<float> offsets);

		virtual ~CameraInterface() = default;
	
	public:
		CameraInterface(ECS& refECS);
		Camera* GetCamera(Entity* ptrEntity);

	private:
		ECS& m_refECS;
	};
}