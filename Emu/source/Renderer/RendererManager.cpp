#pragma once

#include <stdio.h>
#include "../../include/SDLWrapper/SDLWrapper.h"

#include "../../include/Rendering/RendererManager.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Scenes/SceneObject.h"
#include "../../include/Physics/IPhysicsBody.h"
#include "../../include/Rendering/Renderer.h"
#include "../../include/Textures/Texture.h"

namespace Engine
{
	// Initialize static member
	RendererManager* RendererManager::instance = nullptr;

	RendererManager::RendererManager() 
	{}

	void RendererManager::CreateRenderer(SDLWindow* window)
	{
		std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(window);
		m_renderer = std::static_pointer_cast<IRenderer>(renderer);

		SetViewport(window);
	}

	RendererManager* RendererManager::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new RendererManager();
		}
		return instance;
	}

	std::shared_ptr<IRenderer> RendererManager::GetRenderer() const
	{
		return m_renderer;
	}

	// Wrapper for SDL_RenderClear. Clears the screen.
	void RendererManager::ClearScreen()
	{
		m_renderer->ClearScreen();
	}

	// Wrapper for SDL_RenderPresent. Talks to the actual hardwares renderer to display the renderer.
	void RendererManager::Display()
	{
		m_renderer->Display();
	}

	// Definition of render function for the RendererManager class. Takes a SDL_Rect reference which will be rendered.
	void RendererManager::Render(SceneObject* sceneObject, const int pixelsPerMeter, const double interpolation)
	{
		m_renderer->Render(sceneObject, pixelsPerMeter, interpolation);
	}

	void RendererManager::SetViewport(SDLWindow* ptrWindow)
	{
		m_renderer->SetViewport(ptrWindow);
	}

	void RendererManager::free()
	{
		m_renderer->free();
	}

	// Definition for the RendererManager destructor. Calls the free function.
	RendererManager::~RendererManager()
	{
		free();
	}
}