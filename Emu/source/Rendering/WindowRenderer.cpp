#pragma once

#include <memory>
#include "../../include/ISDL/ISDL.h"

#include "../../include/Logging/Logger.h"

#include "../../include/CallbackSystem/CallbackSystem.h"
#include "../../include/Rendering/WindowRenderer.h"

#include "../../include/ComponentManager/ComponentManager.h"
#include "../../include/Transform.h"

namespace Engine
{
	Vector2D<int> Screen::VIEWPORT_SIZE = Vector2D<int>(0, 0);
	Vector2D<int> Screen::VIEWPORT_POSITION = Vector2D<int>(0, 0);
	Vector2D<float> Screen::SCALE = Vector2D<float>(0.0f, 0.0f);
	float Screen::SCALE_CONSTANT = 0.0f;
	Vector2D<int> Screen::SCREEN_SIZE = Vector2D<int>(0, 0);
	Vector2D<int> Screen::VIRTUAL_SIZE = Vector2D<int>(1280, 720);

	WindowRenderer::WindowRenderer() : m_rendererCreated(false), m_ptrWindow(nullptr), m_ptrRenderer(nullptr), ptrCurrentCamera(nullptr)
	{
		ENGINE_CRITICAL("Creating Renderer");

		if (m_rendererCreated)
		{
			ENGINE_INFO_D("Renderer already created. Returning.");
			return;
		}

		// Create window
		m_ptrWindow = ISDL::CreateWindow(
			"DEFAULT WINDOW", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, 
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP
		);

		if (m_ptrWindow == nullptr)
		{
			ENGINE_CRITICAL("Window not created! SDL_Error: " + std::string(ISDL::GetError()));
		}

		// Gotta be an easier way?
		// Is there a way to get the full screen size without toggling fullscreen by default?
		SDLDisplayMode displayMode;
		if (ISDL::GetDesktopDisplayMode(0, &displayMode) != 0)
		{
			ENGINE_CRITICAL_D("Get desktop display mode failed: " + std::string(ISDL::GetError()));
		}

		Screen::SCREEN_SIZE.X = displayMode.w;
		Screen::SCREEN_SIZE.Y = displayMode.h;


		// Create renderer

		// renderer = SDL_CREATE_RENDERER(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // rendering driver?
		m_ptrRenderer = ISDL::CreateRenderer((SDLWindow*)m_ptrWindow, -1, SDL_RENDERER_ACCELERATED);
		if (m_ptrRenderer == nullptr)
		{
			ENGINE_CRITICAL("Renderer could not be created! SDL Error: " + std::string(ISDL::GetError()));
		}

		ISDL::SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 64, 64, 64, SDL_ALPHA_OPAQUE);

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

	void WindowRenderer::SetCamera(Camera* currentCamera)
	{
		ptrCurrentCamera = currentCamera;
	}

	void WindowRenderer::RenderScene(std::shared_ptr<Scene> currentScene, const double interpolation)
	{

		ClearScreen();

		// Calculate camera bounds
		float cameraLeft = ptrCurrentCamera->m_offset.X;
		float cameraRight = ptrCurrentCamera->m_offset.X + (Screen::VIEWPORT_SIZE.X / ptrCurrentCamera->GetPixelsPerUnit());
		float cameraTop = ptrCurrentCamera->m_offset.Y;
		float cameraBottom = ptrCurrentCamera->m_offset.Y + (Screen::VIEWPORT_SIZE.Y / ptrCurrentCamera->GetPixelsPerUnit());

		// Objects should be submitted for rendering instead of iterating through every scene object. ECS would solve this.
		for (auto& layer : currentScene->GetLayers())
		{
			for (auto& sceneObjectID : layer)
			{
				// if (!sceneObject->Visible) continue;

				Transform* transform = ComponentManagerRegistry::GetManager<Transform>().GetComponent(sceneObjectID);

				if (transform == nullptr)
				{
					continue;
				}

				float objectLeft = transform->Position.X;
				float objectRight = objectLeft + transform->Dimensions.X;
				float objectTop = transform->Position.Y;
				float objectBottom = objectTop + transform->Dimensions.Y;

				bool isVisible = objectRight >= cameraLeft && objectLeft <= cameraRight &&
				objectBottom >= cameraTop && objectTop <= cameraBottom;

				if (isVisible)
				{
					Draw(transform, ptrCurrentCamera->GetPixelsPerUnit(), interpolation, Vector2D<float>(cameraLeft, cameraTop));
				}
				else if (sceneObjectID == 1)
				{
					ENGINE_CRITICAL_D("Player not visible!");
				}
			}
		}

		Display();
	}

	// Definition of render function for the RendererManager class. Takes a SDL_Rect reference which will be rendered.
	void WindowRenderer::Draw(Transform* transform, const int pixelsPerUnit, const double interpolation, const Vector2D<float> offset)
	{
		// The x, y, height, and width of the portion of the texture we want to render.
		SDLRect src = { 0, 0, 0, 0 };

		SDLRect dst
		{
			static_cast<int>(round((Lerp(transform->PrevPosition.X, transform->Position.X, \
				(float)interpolation) - offset.X) * pixelsPerUnit * Screen::SCALE_CONSTANT)),
			static_cast<int>(round((Lerp(transform->PrevPosition.Y, transform->Position.Y, 
				(float)interpolation) - offset.Y) * pixelsPerUnit * Screen::SCALE_CONSTANT)),

			static_cast<int>(round(transform->Dimensions.X * pixelsPerUnit * Screen::SCALE_CONSTANT)),
			static_cast<int>(round(transform->Dimensions.Y * pixelsPerUnit * Screen::SCALE_CONSTANT))
		};

		ISDL::RenderCopyEx((SDLRenderer*)m_ptrRenderer, nullptr, nullptr, &dst, transform->Rotation, nullptr, SDL_FLIP_NONE);

		// This should show the boundary of the physics body, not the texture.
#if defined(DEBUG)
		/*if (ptrBody->GetHasCollisionBelow() || ptrBody->GetHasCollisionAbove() ||
			ptrBody->GetHasCollisionLeft() || ptrBody->GetHasCollisionRight())
		{
			SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 0, 0, 255, 255);
		}
		else if (ptrBody->GetHasSensorBelow() || ptrBody->GetHasSensorAbove() ||
			ptrBody->GetHasSensorLeft() || ptrBody->GetHasSensorRight())
		{
			SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 0, 255, 0, 255);
		}
		else*/
		{
			SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 255, 0, 0, 255);
		}
		SDL_RenderDrawRect((SDLRenderer*)m_ptrRenderer, &dst);
		SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 64, 64, 64, SDL_ALPHA_OPAQUE);
#endif
	}

	// Wrapper for SDL_RenderPresent. Talks to the actual hardwares renderer to display the renderer.
	void WindowRenderer::Display()
	{
		ISDL::RenderPresent((SDLRenderer*)m_ptrRenderer);
	}

	// Wrapper for SDL_RenderClear. Clears the screen.
	void WindowRenderer::ClearScreen()
	{
		ISDL::RenderClear((SDLRenderer*)m_ptrRenderer);
	}

	void WindowRenderer::ResizeWindow(const int newWindowWidth, const int newWindowHeight)
	{
		ENGINE_TRACE_D(std::to_string(newWindowWidth) + ", " + std::to_string(newWindowHeight));

		if (newWindowWidth < Screen::SCREEN_SIZE.X / 2 && newWindowHeight < Screen::SCREEN_SIZE.Y / 2)
		{
			ISDL::SetWindowSize((SDLWindow*)m_ptrWindow, Screen::SCREEN_SIZE.X / 2, Screen::SCREEN_SIZE.Y / 2);
		}
		else if (newWindowWidth < Screen::SCREEN_SIZE.X / 2)
		{
			ISDL::SetWindowSize((SDLWindow*)m_ptrWindow, Screen::SCREEN_SIZE.X / 2, newWindowHeight);
		}
		else if (newWindowHeight < Screen::SCREEN_SIZE.Y / 2)
		{
			ISDL::SetWindowSize((SDLWindow*)m_ptrWindow, newWindowWidth, Screen::SCREEN_SIZE.Y / 2);
		}
		else
		{
			// Let OS handle resize.
		}

	}

	void WindowRenderer::SetViewport()
	{
		int windowWidth, windowHeight;
		ISDL::GetWindowSize((SDLWindow*)m_ptrWindow, &windowWidth, &windowHeight);

		Screen::SCALE = Vector2D<float>(static_cast<float>(windowWidth) / Screen::VIRTUAL_SIZE.X, 
			static_cast<float>(windowHeight) / Screen::VIRTUAL_SIZE.Y);

		Screen::SCALE_CONSTANT = std::min(Screen::SCALE.X, Screen::SCALE.Y);

		Screen::VIEWPORT_SIZE = Vector2D<int>(Screen::VIRTUAL_SIZE.X * (int)Screen::SCALE_CONSTANT, 
			Screen::VIRTUAL_SIZE.Y * (int)Screen::SCALE_CONSTANT);

		Screen::VIEWPORT_POSITION.X = (windowWidth - Screen::VIEWPORT_SIZE.X) / 2;
		Screen::VIEWPORT_POSITION.Y = (windowHeight - Screen::VIEWPORT_SIZE.Y) / 2;

		SDLRect viewport = { Screen::VIEWPORT_POSITION.X, Screen::VIEWPORT_POSITION.Y, Screen::VIEWPORT_SIZE.X, Screen::VIEWPORT_SIZE.Y };
		ISDL::RenderSetViewport((SDLRenderer*)m_ptrRenderer, &viewport);
	}

	void WindowRenderer::ToggleFullscreen()
	{
		// Bug here: Figure out why "SDL_WINDOW_FULLSCREEN" does not work.
		// Incompatibility with native video mode?
		bool isFullscreen = ISDL::GetWindowFlags((SDLWindow*)m_ptrWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP;

		// Engine should not support resizing the simulation. That is, the pixels per unit should not change.
		// What will happen when the window size changes is that the camera will center on the player, or 
		// whatever object the camera is locked onto, until it hits the edge of the screen.

		if (ISDL::SetWindowFullscreen((SDLWindow*)m_ptrWindow, isFullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP) < 0)
		{
			ENGINE_ERROR("Fullscreen failed! SDL_Error: " + std::string(ISDL::GetError()));

		}
		else
		{
			// Default behavior for now will be to toggle fullscreen on for client.
			// When the screen is toggled to windowed, the size will be half of the width and height.
			ISDL::SetWindowSize((SDLWindow*)m_ptrWindow, Screen::SCREEN_SIZE.X / 2, Screen::SCREEN_SIZE.Y / 2);
			ISDL::SetWindowPosition((SDLWindow*)m_ptrWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		}
	}

	void WindowRenderer::free()
	{
		ENGINE_INFO("Freeing Renderer.");
		ISDL::DestroyRenderer((SDLRenderer*)m_ptrRenderer);
		m_ptrRenderer = nullptr;

		ENGINE_INFO("Freeing Window.");
		ISDL::DestroyWindow((SDLWindow*)m_ptrWindow);
		m_ptrWindow = nullptr;
	}

	WindowRenderer::~WindowRenderer()
	{
	}
}
