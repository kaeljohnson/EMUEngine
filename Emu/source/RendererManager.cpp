#pragma once

#include <stdio.h>
#include "../include/SDLWrapper/SDLWrapper.h"

#include "../include/RendererManager.h"
#include "../include/Logging/Logger.h"
#include "../include/Scenes/SceneObject.h"
#include "../include/Physics/IPhysicsBody.h"

namespace Engine
{
	// Initialize static member
	RendererManager* RendererManager::instance = nullptr;

	RendererManager::RendererManager() 
		: VIRTUAL_WIDTH(1280), VIRTUAL_HEIGHT(720), SCALE_X(0), SCALE_Y(0), SCALE(0), 
		viewportX(0), viewportY(0), viewportWidth(0), viewportHeight(0),
		rendererCreated(false), renderer(nullptr)
	{}

	void RendererManager::CreateRenderer(SDLWindow* window)
	{
		if (rendererCreated)
		{
			ENGINE_INFO_D("Renderer already created. Returning.");
			return;
		}

		// renderer = SDL_CREATE_RENDERER(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // rendering driver?
		renderer = SDL_CREATE_RENDERER(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == nullptr)
		{
			ENGINE_CRITICAL("Renderer could not be created! SDL Error: {}", SDL_GET_ERROR());
		}

		SDL_SetRenderDrawColor(renderer, 'd3', 'd3', 'd3', SDL_ALPHA_OPAQUE);

		setViewport(window);

		// Renderer only supports one window.
		rendererCreated = true;
	}

	RendererManager* RendererManager::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new RendererManager();
		}
		return instance;
	}

	SDLRenderer* RendererManager::GetRenderer() const
	{
		rendererCreated ? ENGINE_INFO_D("Renderer is created.") : 
						  ENGINE_INFO_D("Renderer is not created. Returning nullptr.");

		return renderer;
	}

	// Definition for loadTexture function in RendererManager. 
	// Returns a const SDL_Texture*. Takes a file (probably a sprite sheet) 
	// full of the textures we want to load.
	const SDLTexture* RendererManager::loadTexture(const char* filePath)
	{
		SDLTexture* texture =  IMG_LoadTexture(renderer, filePath);
		if (texture == nullptr)
		{
			ENGINE_CRITICAL("Failed to load texture. SDL Error: {}", SDL_GET_ERROR());
		}
		return texture;
	}

	// Wrapper for SDL_RenderClear. Clears the screen.
	void RendererManager::clearScreen()
	{
		SDL_RENDER_CLEAR(renderer);
	}

	// Wrapper for SDL_RenderPresent. Talks to the actual hardwares renderer to display the renderer.
	void RendererManager::display()
	{
		SDL_RENDER_PRESENT(renderer);
	}

	// Definition of render function for the RendererManager class. Takes a SDL_Rect reference which will be rendered.
	void RendererManager::render(SceneObject* sceneObject, const int pixelsPerMeter, const double interpolation)
	{
		// The x, y, height, and width of the portion of the texture we want to render.
		SDLRect src = { 0, 0, 0, 0 };

		std::shared_ptr<IPhysicsBody> ptrBody = sceneObject->GetPhysicsBody();

		SDLRect dst
		{
			// This rect will eventually be the outline of the texture we want to render,
			// not the collidable object tracked by the underlying box2d body.

			static_cast<int>(round((ptrBody->GetTopLeftPrevX() * (1.0 - interpolation) + ptrBody->GetTopLeftXInMeters() * interpolation) * pixelsPerMeter * SCALE)),
			static_cast<int>(round((ptrBody->GetTopLeftPrevY() * (1.0 - interpolation) + ptrBody->GetTopLeftYInMeters() * interpolation) * pixelsPerMeter * SCALE)),
			
			static_cast<int>(round(ptrBody->GetWidthInMeters() * pixelsPerMeter * SCALE)),
			static_cast<int>(round(ptrBody->GetHeightInMeters() * pixelsPerMeter * SCALE))
		};

		SDL_RENDER_COPY_EX(renderer, sceneObject->GetTexture()->m_texture, nullptr, &dst, ptrBody->GetAngleInDegrees(), nullptr, SDL_FLIP_NONE);
	}

	void RendererManager::setViewport(SDLWindow* ptrWindow)
	{
		int windowWidth, windowHeight;
		SDL_GET_WINDOW_SIZE(ptrWindow, &windowWidth, &windowHeight);

		SCALE_X = static_cast<float>(windowWidth) / VIRTUAL_WIDTH;
		SCALE_Y = static_cast<float>(windowHeight) / VIRTUAL_HEIGHT;

		SCALE = std::min(SCALE_X, SCALE_Y);

		viewportWidth = static_cast<int>(VIRTUAL_WIDTH * SCALE);
		viewportHeight = static_cast<int>(VIRTUAL_HEIGHT * SCALE);

		viewportX = (windowWidth - viewportWidth) / 2;
		viewportY = (windowHeight - viewportHeight) / 2;

		SDLRect viewport = { viewportX, viewportY, viewportWidth, viewportHeight };
		SDL_RENDER_SET_VIEWPORT(renderer, &viewport);
	}

	void RendererManager::free()
	{
		ENGINE_INFO("Freeing Renderer.");
		// Destroy the render, which SDL uses dynamic memory to allocate.
		SDL_DESTROY_RENDERER(renderer);

		// Now that we freed the renderer, the pointer is still attached to that memory.
		// This is bad because we don't control what is there anymore, so it needs to be set back to nullptr.
		renderer = nullptr;
	}

	// Definition for the RendererManager destructor. Calls the free function.
	RendererManager::~RendererManager()
	{
		free();
	}
}