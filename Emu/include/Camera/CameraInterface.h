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
		EMU_API void Activate(Entity entity);
		EMU_API void SetPixelsPerUnit(Entity entity, const int pixelsPerUnit);
		EMU_API void SetPixelsPerUnit(Camera& body, const int pixelsPerUnit);
		EMU_API const int GetPixelsPerUnit(Entity entity);
		EMU_API const int GetPixelsPerUnit(Camera& body);
		EMU_API void SetOffset(Entity entity, const Vector2D<float> offset);
		EMU_API void SetOffset(Camera& body, const Vector2D<float> offset);
		EMU_API const Vector2D<float> GetOffset(Entity entity);
		EMU_API const Vector2D<float> GetOffset(Camera& body);
		EMU_API void SetClampingOn(Entity entity, const bool clampingOn);
		EMU_API void SetClampingOn(Camera& body, const bool clampingOn);
		EMU_API const bool GetClampingOn(Entity entity);
		EMU_API const bool GetClampingOn(Camera& body);
		EMU_API void SetSize(Entity entity);
		EMU_API void SetSize(Camera& body);
		EMU_API const Vector2D<float> GetSize(Entity entity);
		EMU_API const Vector2D<float> GetSize(Camera& body);
		EMU_API void SetOffsets(Entity entity, const Vector2D<float> offsets);
		EMU_API void SetOffsets(Camera& body, const Vector2D<float> offsets);

		virtual ~CameraInterface() = default;
	
	public:
		CameraInterface(ECS& refECS);
		Camera* GetCamera(Entity entity);

	private:
		ECS& m_refECS;

		size_t m_activeCameraEntity;
	};
}