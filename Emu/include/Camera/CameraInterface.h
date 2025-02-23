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
		EMU_API CameraInterface(ECS& refECS);
		
		EMU_API void SetPixelsPerUnit(Camera& refCamera, const int pixelsPerUnit);
		EMU_API void SetCameraPosition(Camera& ptrEntity, const Vector2D<float> offset);
		EMU_API void SetClampingOn(Camera& ptrEntity, const bool clampingOn);
		EMU_API void SetSize(Camera& ptrEntity);
		EMU_API inline const int GetPixelsPerUnit(Camera& refCamera) const { return refCamera.m_pixelsPerUnit; }
		EMU_API void Frame(Camera& refCamera, const Vector2D<int> mapBounds);

		virtual ~CameraInterface() = default;

	private:
		ECS& m_refECS;
	};
}