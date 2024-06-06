#pragma once

#include "../Textures/Texture.h"
#include "../Scenes/SceneObject.h"

namespace Engine
{
	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;
		virtual void SetViewport(SDLWindow* ptrWindow) = 0;
		virtual void ClearScreen() = 0;
		virtual void Render(SceneObject* ptrSceneObject, const int pixelsPerMeter, const double interpolation) = 0;
		virtual void Display() = 0;
		virtual void free() = 0;
	};
}