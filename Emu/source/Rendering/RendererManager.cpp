#pragma once

#include <memory>

#include "../../include/SDLWrapper/SDLWrapper.h"

#include "../../include/Rendering/RendererManager.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Scenes/Scene.h"
#include "../../include/Scenes/SceneObject.h"
#include "../../include/Physics/PhysicsBody.h"

#include "../../include/Rendering/WindowRenderer.h"

#include "../../include/MathUtil.h"

namespace Engine
{
	RendererManager::RendererManager() : m_ptrCurrentScene(nullptr),
		m_ptrWindowRenderer(std::make_unique<WindowRenderer>())
	{}
	
	void RendererManager::ToggleFullscreen()
	{
		m_ptrWindowRenderer->ToggleFullscreen();
	}

	void RendererManager::ResizeWindow(const int newWindowWidth, const int newWindowHeight)
	{
		m_ptrWindowRenderer->ResizeWindow(newWindowWidth, newWindowHeight);
	}

	void RendererManager::RenderScene(const double interpolation, const Vector2D<float> cameraOffset)
	{
		m_ptrWindowRenderer->RenderScene(m_ptrCurrentScene, interpolation, cameraOffset);

	}

	void RendererManager::SetScene(std::shared_ptr<Scene> ptrScene)
	{
		m_ptrCurrentScene = ptrScene;
	}

	void RendererManager::SetViewport()
	{
		m_ptrWindowRenderer->SetViewport();
	}

	const Vector2D<float> RendererManager::GetScale() const
	{
		return m_ptrWindowRenderer->m_scale;
	}

	const int RendererManager::GetFullscreenWidth() const { return m_ptrWindowRenderer->m_screenSize.X; }
	const int RendererManager::GetFullscreenHeight() const { return m_ptrWindowRenderer->m_screenSize.Y; }

	void RendererManager::free()
	{
		m_ptrWindowRenderer->free();
	}

	// Definition for the RendererManager destructor. Calls the free function.
	RendererManager::~RendererManager()
	{
		free();
	}
}