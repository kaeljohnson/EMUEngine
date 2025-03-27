#pragma once

#include "../../include/ISDL/ISDL.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Rendering/WindowRenderer.h"
#include "../../include/Camera/CameraInterface.h"
#include "../../include/ECS/ECS.h"
#include "../../include/Components.h"
#include "../../include/Time.h"

#include <chrono>

namespace Engine
{
	bool Screen::WINDOW_RESIZE_REQUEST = false;
	bool Screen::TOGGLE_FULLSCREEN_REQUEST = false;
	Vector2D<int> Screen::VIEWPORT_SIZE = Vector2D<int>(0, 0);
	Vector2D<int> Screen::VIEWPORT_POSITION = Vector2D<int>(0, 0);
	Vector2D<float> Screen::SCALE = Vector2D<float>(0.0f, 0.0f);
	float Screen::SCALE_CONSTANT = 0.0f;
	Vector2D<int> Screen::SCREEN_SIZE = Vector2D<int>(0, 0);
	Vector2D<int> Screen::VIRTUAL_SIZE = Vector2D<int>(1280, 720);

	WindowRenderer::WindowRenderer(ECS& refECS) 
		: m_rendererCreated(false), m_ptrWindow(nullptr), m_ptrRenderer(nullptr), m_refECS(refECS)
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
	}

	// The renderer being responbible for sorting the etities to render is not a modular design.
	// Ideally, they are submitted to a render pipeline in the correct order but this will do for now.
	void WindowRenderer::Initialize()
	{
		// Is there a better data structure than a vector for this?
		m_sortedEntitiesToRender.clear();

		// Get active transform components
		auto& activeTransforms = m_refECS.GetHotComponents<Transform>();

		// Reserve space to avoid unnecessary allocations
		m_sortedEntitiesToRender.reserve(activeTransforms.size());

		// Store entity IDs
		for (const auto& transform : activeTransforms)
		{
			m_sortedEntitiesToRender.push_back(transform.m_entity);
		}

		// Sort entity IDs based on their Z-index in descending order
		std::sort(m_sortedEntitiesToRender.begin(), m_sortedEntitiesToRender.end(),
			[&](size_t entityA, size_t entityB)
			{
				const Transform* transformA = m_refECS.GetComponent<Transform>(entityA);
				const Transform* transformB = m_refECS.GetComponent<Transform>(entityB);
				return transformA->ZIndex > transformB->ZIndex; // Highest Z first
			});
	}

	void WindowRenderer::Activate(Entity entity)
	{
		// Find the correct position using binary search
		auto insertPos = std::lower_bound(
			m_sortedEntitiesToRender.begin(), m_sortedEntitiesToRender.end(),
			entity,
			[&](size_t existingEntity, size_t newEntity)
			{
				return m_refECS.GetComponent<Transform>(existingEntity)->ZIndex >
					m_refECS.GetComponent<Transform>(newEntity)->ZIndex;
			});

		// Insert the entity at the correct position
		m_sortedEntitiesToRender.insert(insertPos, entity);
	}

	void WindowRenderer::Deactivate(Entity entity)
	{
		// Find the entity in the sorted list
		auto it = std::find(m_sortedEntitiesToRender.begin(), m_sortedEntitiesToRender.end(), entity);

		// Remove the entity from the sorted list
		if (it != m_sortedEntitiesToRender.end())
		{
			m_sortedEntitiesToRender.erase(it);
		}
	}

	void WindowRenderer::Render()
	{
		ClearScreen();

		Camera* ptrCurrentCamera = nullptr;

		// Get the first active camera for now.
		for (auto& camera : m_refECS.GetHotComponents<Camera>())
		{
			ptrCurrentCamera = &camera;
			break;
		}

		if (ptrCurrentCamera == nullptr)
		{
			ENGINE_CRITICAL("No active camera found. Returning.");
			return;
		}

		if (Screen::WINDOW_RESIZE_REQUEST)
		{
			ResizeWindow(Screen::SCREEN_SIZE.X, Screen::SCREEN_SIZE.Y);
			SetViewport();
			Screen::WINDOW_RESIZE_REQUEST = false;
		}

		if (Screen::TOGGLE_FULLSCREEN_REQUEST)
		{
			ToggleFullscreen();
			SetViewport();
			Screen::TOGGLE_FULLSCREEN_REQUEST = false;
		}

		// Calculate camera bounds
		float cameraLeft = ptrCurrentCamera->m_offset.X;
		float cameraRight = ptrCurrentCamera->m_offset.X + (Screen::VIEWPORT_SIZE.X / ptrCurrentCamera->m_pixelsPerUnit);
		float cameraTop = ptrCurrentCamera->m_offset.Y;
		float cameraBottom = ptrCurrentCamera->m_offset.Y + (Screen::VIEWPORT_SIZE.Y / ptrCurrentCamera->m_pixelsPerUnit);

		
		auto start = std::chrono::high_resolution_clock::now();

		// This is half as fast as below loop. Ordered by z index
		/*for (Entity& entity : m_sortedEntitiesToRender)
		{
			Transform* ptrTransform = m_refECS.GetComponent<Transform>(entity);

			float objectLeft = ptrTransform->Position.X;
			float objectRight = objectLeft + ptrTransform->Dimensions.X;
			float objectTop = ptrTransform->Position.Y;
			float objectBottom = objectTop + ptrTransform->Dimensions.Y;

			bool isVisible = objectRight >= cameraLeft && objectLeft <= cameraRight &&
				objectBottom >= cameraTop && objectTop <= cameraBottom;

			if (isVisible)
			{
				Draw(*ptrTransform, ptrCurrentCamera->m_pixelsPerUnit, Vector2D<float>(cameraLeft, cameraTop));
			}
		}*/

		// This is twice as fast as above loop. Not ordered
		auto& transformManager = m_refECS.GetHotComponents<Transform>();
		for (Transform& refTransform : transformManager)
		{
			float objectLeft = refTransform.Position.X;
			float objectRight = objectLeft + refTransform.Dimensions.X;
			float objectTop = refTransform.Position.Y;
			float objectBottom = objectTop + refTransform.Dimensions.Y;

			bool isVisible = objectRight >= cameraLeft && objectLeft <= cameraRight &&
				objectBottom >= cameraTop && objectTop <= cameraBottom;

			if (isVisible)
			{
				Draw(refTransform, ptrCurrentCamera->m_pixelsPerUnit, Vector2D<float>(cameraLeft, cameraTop));
			}
		}
		auto end = std::chrono::high_resolution_clock::now();
		std::cout << "Rendering: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";

		Display();
	}

	void WindowRenderer::Draw(Transform& transform, const int pixelsPerUnit, const Vector2D<float> offset)
	{
		const float interpolation = Time::GetInterpolationFactor();

		// The x, y, height, and width of the portion of the texture we want to render.
		SDLRect src = { 0, 0, 0, 0 };

		SDLRect dst
		{
			static_cast<int>(round((Lerp(transform.PrevPosition.X, transform.Position.X, 
				interpolation) - offset.X) * pixelsPerUnit * Screen::SCALE_CONSTANT)),
			static_cast<int>(round((Lerp(transform.PrevPosition.Y, transform.Position.Y, 
				interpolation) - offset.Y) * pixelsPerUnit * Screen::SCALE_CONSTANT)),

			static_cast<int>(round(transform.Dimensions.X * pixelsPerUnit * Screen::SCALE_CONSTANT)),
			static_cast<int>(round(transform.Dimensions.Y * pixelsPerUnit * Screen::SCALE_CONSTANT))
		};

		ISDL::RenderCopyEx((SDLRenderer*)m_ptrRenderer, nullptr, nullptr, &dst, transform.Rotation, nullptr, SDL_FLIP_NONE);

		// This should show the boundary of the physics body, not the texture.

		SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 255, 0, 0, 255);
		SDL_RenderDrawRect((SDLRenderer*)m_ptrRenderer, &dst);
		SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 64, 64, 64, SDL_ALPHA_OPAQUE);
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
