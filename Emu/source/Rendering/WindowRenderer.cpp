#pragma once

#include <memory>

#include "../../include/Logging/Logger.h"

#include "../../include/CallbackSystem/CallbackSystem.h"
#include "../../include/Rendering/WindowRenderer.h"

namespace Engine
{
	WindowRenderer::WindowRenderer() : m_virtualSize(1280, 720), m_scale(0, 0), SCALE(0),
		m_viewportPosition(0, 0), m_viewportSize(0, 0), m_screenSize(0, 0), m_rendererCreated(false), m_ptrWindow(nullptr), m_ptrRenderer(nullptr)
	{
		ENGINE_CRITICAL("Creating Renderer");

		if (m_rendererCreated)
		{
			ENGINE_INFO_D("Renderer already created. Returning.");
			return;
		}

		// Create window
		m_ptrWindow = SDL_CREATE_WINDOW(
			"DEFAULT WINDOW", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP
		);

		if (m_ptrWindow == nullptr)
		{
			ENGINE_CRITICAL("Window not created! SDL_Error: " + std::string(SDL_GET_ERROR()));
		}

		// Gotta be an easier way?
		// Is there a way to get the full screen size without toggling fullscreen by default?
		SDLDisplayMode displayMode;
		if (SDL_GET_DESKTOP_DISPLAY_MODE(0, &displayMode) != 0)
		{
			ENGINE_CRITICAL_D("Get desktop display mode failed: " + std::string(SDL_GET_ERROR()));
		}

		m_screenSize.X = displayMode.w;
		m_screenSize.Y = displayMode.h;


		// Create renderer

		// renderer = SDL_CREATE_RENDERER(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // rendering driver?
		m_ptrRenderer = SDL_CREATE_RENDERER(m_ptrWindow, -1, SDL_RENDERER_ACCELERATED);
		if (m_ptrRenderer == nullptr)
		{
			ENGINE_CRITICAL("Renderer could not be created! SDL Error: " + std::string(SDL_GET_ERROR()));
		}

		SDL_SetRenderDrawColor(m_ptrRenderer, 64, 64, 64, SDL_ALPHA_OPAQUE);

		SetViewport();

		// Renderer only supports one window.
		m_rendererCreated = true;

		ENGINE_CRITICAL("Renderer created.");

		ICallbackSystem::GetInstance()->NewCallback(Type::ToggleFullscreen, [&](Data data)
			{
				ToggleFullscreen();
				SetViewport();
			});

		ICallbackSystem::GetInstance()->NewCallback(Type::ResizeWindow, [&](Data data)
			{
				const std::pair<int, int> windowSize = std::get<const std::pair<int, int>>(data);

				ResizeWindow(windowSize.first, windowSize.second);
				SetViewport();
			});
	}

	void WindowRenderer::RenderScene(std::shared_ptr<Scene> currentScene, const double interpolation, const Math::Vector2D<float> cameraOffset)
	{

		ClearScreen();

		// Calculate camera bounds
		float cameraLeft = cameraOffset.X;
		float cameraRight = cameraOffset.X + ( m_viewportSize.X / currentScene->GetPixelsPerUnit());
		float cameraTop = cameraOffset.Y;
		float cameraBottom = cameraOffset.Y + (m_viewportSize.Y / currentScene->GetPixelsPerUnit());

		for (auto& layer : currentScene->GetLayers())
		{
			for (auto& sceneObject : layer)
			{
				float objectLeft = sceneObject->GetPhysicsBody()->GetTopLeftPosition().X;
				float objectRight = objectLeft + sceneObject->GetPhysicsBody()->GetDimensions().X;
				float objectTop = sceneObject->GetPhysicsBody()->GetTopLeftPosition().Y;
				float objectBottom = objectTop + sceneObject->GetPhysicsBody()->GetDimensions().Y;

				bool isVisible = objectRight >= cameraLeft && objectLeft <= cameraRight &&
					objectBottom >= cameraTop && objectTop <= cameraBottom;

				if (isVisible)
				{
					Draw(sceneObject, currentScene->GetPixelsPerUnit(), interpolation, Math::Vector2D<float>(cameraLeft, cameraTop));
				}
			}
		}

		Display();
	}

	// Definition of render function for the RendererManager class. Takes a SDL_Rect reference which will be rendered.
	void WindowRenderer::Draw(SceneObject* sceneObject, const int pixelsPerUnit, const double interpolation, const Math::Vector2D<float> offset)
	{
		// The x, y, height, and width of the portion of the texture we want to render.
		SDLRect src = { 0, 0, 0, 0 };

		std::shared_ptr<PhysicsBody> ptrBody = sceneObject->GetPhysicsBody();

		SDLRect dst
		{
			static_cast<int>(round((Math::Lerp(ptrBody->GetTopLeftPrevPosition().X, ptrBody->GetTopLeftPosition().X, (float)interpolation) - offset.X) * pixelsPerUnit * SCALE)),
			static_cast<int>(round((Math::Lerp(ptrBody->GetTopLeftPrevPosition().Y, ptrBody->GetTopLeftPosition().Y, (float)interpolation) - offset.Y) * pixelsPerUnit * SCALE)),

			static_cast<int>(round(ptrBody->GetDimensions().X * pixelsPerUnit * SCALE)),
			static_cast<int>(round(ptrBody->GetDimensions().Y * pixelsPerUnit * SCALE))
		};

		SDL_RENDER_COPY_EX(m_ptrRenderer, nullptr, nullptr, &dst, ptrBody->GetAngleInDegrees(), nullptr, SDL_FLIP_NONE);

		// This should show the boundary of the physics body, not the texture.
#if defined(DEBUG)
		if (ptrBody->GetHasCollisionBelow() || ptrBody->GetHasCollisionAbove() ||
			ptrBody->GetHasCollisionLeft() || ptrBody->GetHasCollisionRight())
		{
			SDL_SetRenderDrawColor(m_ptrRenderer, 0, 0, 255, 255);
		}
		else if (ptrBody->GetHasSensorBelow() || ptrBody->GetHasSensorAbove() ||
			ptrBody->GetHasSensorLeft() || ptrBody->GetHasSensorRight())
		{
			SDL_SetRenderDrawColor(m_ptrRenderer, 0, 255, 0, 255);
		}
		else
		{
			SDL_SetRenderDrawColor(m_ptrRenderer, 255, 0, 0, 255);
		}
		SDL_RenderDrawRect(m_ptrRenderer, &dst);
		SDL_SetRenderDrawColor(m_ptrRenderer, 64, 64, 64, SDL_ALPHA_OPAQUE);
#endif
	}

	// Wrapper for SDL_RenderPresent. Talks to the actual hardwares renderer to display the renderer.
	void WindowRenderer::Display()
	{
		SDL_RENDER_PRESENT(m_ptrRenderer);
	}

	// Wrapper for SDL_RenderClear. Clears the screen.
	void WindowRenderer::ClearScreen()
	{
		SDL_RENDER_CLEAR(m_ptrRenderer);
	}

	void WindowRenderer::ResizeWindow(const int newWindowWidth, const int newWindowHeight)
	{
		ENGINE_TRACE_D(std::to_string(newWindowWidth) + ", " + std::to_string(newWindowHeight));

		if (newWindowWidth < m_screenSize.X / 2 && newWindowHeight < m_screenSize.Y / 2)
		{
			SDL_SET_WINDOW_SIZE(m_ptrWindow, m_screenSize.X / 2, m_screenSize.Y / 2);
		}
		else if (newWindowWidth < m_screenSize.X / 2)
		{
			SDL_SET_WINDOW_SIZE(m_ptrWindow, m_screenSize.X / 2, newWindowHeight);
		}
		else if (newWindowHeight < m_screenSize.Y / 2)
		{
			SDL_SET_WINDOW_SIZE(m_ptrWindow, newWindowWidth, m_screenSize.Y / 2);
		}
		else
		{
			// Let OS handle resize.
		}

	}

	void WindowRenderer::SetViewport()
	{
		int windowWidth, windowHeight;
		SDL_GET_WINDOW_SIZE(m_ptrWindow, &windowWidth, &windowHeight);

		m_scale.X = static_cast<float>(windowWidth) / m_virtualSize.X;
		m_scale.Y = static_cast<float>(windowHeight) / m_virtualSize.Y;

		SCALE = std::min(m_scale.X, m_scale.Y);

		m_viewportSize.X = static_cast<int>(m_virtualSize.X * SCALE);
		m_viewportSize.Y = static_cast<int>(m_virtualSize.Y * SCALE);

		m_viewportPosition.X = (windowWidth - m_viewportSize.X) / 2;
		m_viewportPosition.Y = (windowHeight - m_viewportSize.Y) / 2;

		SDLRect viewport = { m_viewportPosition.X, m_viewportPosition.Y, m_viewportSize.X, m_viewportSize.Y };
		SDL_RENDER_SET_VIEWPORT(m_ptrRenderer, &viewport);
	}

	void WindowRenderer::ToggleFullscreen()
	{
		// Bug here: Figure out why "SDL_WINDOW_FULLSCREEN" does not work.
		// Incompatibility with native video mode?
		bool isFullscreen = SDL_GET_WINDOW_FLAGS(m_ptrWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP;

		// Engine should not support resizing the simulation. That is, the pixels per unit should not change.
		// What will happen when the window size changes is that the camera will center on the player, or 
		// whatever object the camera is locked onto, until it hits the edge of the screen.

		if (SDL_SET_WINDOW_FULLSCREEN(m_ptrWindow, isFullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP) < 0)
		{
			ENGINE_ERROR("Fullscreen failed! SDL_Error: " + std::string(SDL_GET_ERROR()));

		}
		else
		{
			// Default behavior for now will be to toggle fullscreen on for client.
			// When the screen is toggled to windowed, the size will be half of the width and height.
			SDL_SET_WINDOW_SIZE(m_ptrWindow, m_screenSize.X / 2, m_screenSize.Y / 2);
			SDL_SET_WINDOW_POSITION(m_ptrWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		}
	}

	void WindowRenderer::free()
	{
		ENGINE_INFO("Freeing Renderer.");
		SDL_DESTROY_RENDERER(m_ptrRenderer);
		m_ptrRenderer = nullptr;

		ENGINE_INFO("Freeing Window.");
		SDL_DESTROY_WINDOW(m_ptrWindow);
		m_ptrWindow = nullptr;
	}

	WindowRenderer::~WindowRenderer()
	{
	}
}
