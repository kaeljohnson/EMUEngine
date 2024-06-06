#pragma once

#include "../../include/Rendering/Renderer.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{

	Renderer::Renderer(SDLWindow* window) : VIRTUAL_WIDTH(1280), VIRTUAL_HEIGHT(720), SCALE_X(0), SCALE_Y(0), SCALE(0),
		viewportX(0), viewportY(0), viewportWidth(0), viewportHeight(0), renderer(nullptr), rendererCreated(false)
	{
		renderer = SDL_CREATE_RENDERER(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == nullptr)
		{
			ENGINE_CRITICAL("Renderer could not be created! SDL Error: {}", SDL_GET_ERROR());
		}

		rendererCreated = true;

		SDL_SetRenderDrawColor(renderer, 'd3', 'd3', 'd3', SDL_ALPHA_OPAQUE);

		if (rendererCreated)
		{
			ENGINE_INFO_D("Renderer already created. Returning.");
			return;
		}
	}

	void Renderer::Render(SceneObject* ptrSceneObject, const int pixelsPerMeter, const double interpolation)
	{
		// The x, y, height, and width of the portion of the texture we want to render.
		SDLRect src = { 0, 0, 0, 0 };

		std::shared_ptr<IPhysicsBody> ptrBody = ptrSceneObject->GetPhysicsBody();

		SDLRect dst
		{
			// This rect will eventually be the outline of the texture we want to render,
			// not the collidable object tracked by the underlying box2d body.

			static_cast<int>(round((ptrBody->GetTopLeftPrevX() * (1.0 - interpolation) + ptrBody->GetTopLeftXInMeters() * interpolation) * pixelsPerMeter * SCALE)),
			static_cast<int>(round((ptrBody->GetTopLeftPrevY() * (1.0 - interpolation) + ptrBody->GetTopLeftYInMeters() * interpolation) * pixelsPerMeter * SCALE)),

			static_cast<int>(round(ptrBody->GetWidthInMeters() * pixelsPerMeter * SCALE)),
			static_cast<int>(round(ptrBody->GetHeightInMeters() * pixelsPerMeter * SCALE))
		};

		SDLTexture* texture = static_cast<Texture*>(ptrSceneObject->GetTexture().get())->m_texture;

		SDL_RENDER_COPY_EX(renderer, texture, nullptr, &dst, ptrBody->GetAngleInDegrees(), nullptr, SDL_FLIP_NONE);
	}

	void Renderer::SetViewport(SDLWindow* ptrWindow)
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

	void Renderer::ClearScreen()
	{
		SDL_RENDER_CLEAR(renderer);
	}

	void Renderer::Display()
	{
		SDL_RENDER_PRESENT(renderer);
	}

	void Renderer::free()
	{
		ENGINE_INFO("Freeing Renderer.");
		// Destroy the render, which SDL uses dynamic memory to allocate.
		SDL_DESTROY_RENDERER(renderer);

		// Now that we freed the renderer, the pointer is still attached to that memory.
		// This is bad because we don't control what is there anymore, so it needs to be set back to nullptr.
		renderer = nullptr;
	}

	Renderer::~Renderer()
	{
		SDL_DESTROY_RENDERER(renderer);
	}

}