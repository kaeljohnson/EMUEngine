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
		
		
		EMU_API void SetPixelsPerUnit(Entity* ptrEntity, const int pixelsPerUnit);
		EMU_API void SetCameraPosition(Entity* ptrEntity, const Vector2D<float> offset);
		EMU_API void SetClampingOn(Entity* ptrEntity, const bool clampingOn);
		EMU_API void SetSize(Entity* ptrEntity);
		EMU_API const Vector2D<float> GetSize(Entity* ptrEntity);
		EMU_API void SetOffsets(Entity* ptrEntity, const Vector2D<float> offsets);
		EMU_API const Vector2D<float> GetOffsets(Entity* ptrEntity);
		EMU_API inline const int GetPixelsPerUnit(Entity* ptrEntity) const { return m_refECS.GetComponent<Camera>(ptrEntity)->m_pixelsPerUnit; }

		virtual ~CameraInterface() = default;
	
	public:
		CameraInterface(ECS& refECS);
		Camera* GetCamera(Entity* ptrEntity);

	private:
		ECS& m_refECS;
	};
}