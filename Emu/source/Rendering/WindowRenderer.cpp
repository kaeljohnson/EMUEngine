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

	WindowRenderer::WindowRenderer(ECS& refECS, AssetManager& refAssetManager) 
		: m_rendererCreated(false), m_ptrWindow(nullptr), m_ptrRenderer(nullptr), m_refECS(refECS), m_refAssetManager(refAssetManager)
	{
		ENGINE_INFO_D("Creating Renderer");

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

		ENGINE_INFO_D("Window created!");

		// Gotta be an easier way?
		// Is there a way to get the full screen size without toggling fullscreen by default?
		SDLDisplayMode displayMode;
		if (ISDL::GetDesktopDisplayMode(0, &displayMode) != 0)
		{
			ENGINE_CRITICAL("Get desktop display mode failed: " + std::string(ISDL::GetError()));
		}

		Screen::SCREEN_SIZE.X = displayMode.w;
		Screen::SCREEN_SIZE.Y = displayMode.h;


		// Create renderer

		// m_ptrRenderer = SDL_CREATE_RENDERER(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // rendering driver?
		m_ptrRenderer = ISDL::CreateRenderer((SDLWindow*)m_ptrWindow, -1, SDL_RENDERER_ACCELERATED);
		if (m_ptrRenderer == nullptr)
		{
			ENGINE_CRITICAL("Renderer could not be created! SDL Error: " + std::string(ISDL::GetError()));
		}

		ISDL::SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 64, 64, 64, SDL_ALPHA_OPAQUE);

		SetViewport();

		// Renderer only supports one window.
		m_rendererCreated = true;

		// Give renderer pointer to asset manager so it can load textures.
		m_refAssetManager.GiveRenderer(m_ptrRenderer);

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

	void WindowRenderer::Render()
	{
		ClearScreen();

		Camera* ptrCurrentCamera = nullptr;

		if (m_refECS.GetHotComponents<Camera>().empty())
		{
			ENGINE_CRITICAL_D("No active cameras found.");
			throw std::runtime_error("No active cameras found.");
			return;
		}

		// Render cameras in order of array for now. Will change to render in order of Z index later.
		for (auto& camera : m_refECS.GetHotComponents<Camera>())
		{
			ptrCurrentCamera = &camera;

			if (ptrCurrentCamera == nullptr)
			{
				ENGINE_CRITICAL_D("No active camera found. Remember to change camera before deactivating entity.");
				throw std::runtime_error("No active camera found.");
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

			// viewport size in tiles
			float viewportSizeInTilesX = Screen::VIEWPORT_SIZE.X / (ptrCurrentCamera->m_pixelsPerUnit * Screen::SCALE.X);
			float viewportSizeInTilesY = Screen::VIEWPORT_SIZE.Y / (ptrCurrentCamera->m_pixelsPerUnit * Screen::SCALE.Y);

			// Calculate the frame and corresponding camera offset on screen.
			float leftOffset = ptrCurrentCamera->m_offset.X - ptrCurrentCamera->m_position.X * viewportSizeInTilesX;
			float topOffset = ptrCurrentCamera->m_offset.Y - ptrCurrentCamera->m_position.Y * viewportSizeInTilesY;

			// Now the frame can be set according to the original offsets 
			float leftFrame = ptrCurrentCamera->m_offset.X;
			float topFrame = ptrCurrentCamera->m_offset.Y;

			float rightFrame = ptrCurrentCamera->m_offset.X + ptrCurrentCamera->m_screenRatio.X * viewportSizeInTilesX;
			float bottomFrame = ptrCurrentCamera->m_offset.Y + ptrCurrentCamera->m_screenRatio.Y * viewportSizeInTilesY;

			float frameWidth = rightFrame - leftFrame;
			float frameHeight = bottomFrame - topFrame;

			SDL_Rect clipRect;
			clipRect.x = static_cast<int>(ptrCurrentCamera->m_position.X * Screen::VIEWPORT_SIZE.X);
			clipRect.y = static_cast<int>(ptrCurrentCamera->m_position.Y * Screen::VIEWPORT_SIZE.Y);
			clipRect.w = static_cast<int>(frameWidth * ptrCurrentCamera->m_pixelsPerUnit * Screen::SCALE.X);
			clipRect.h = static_cast<int>(frameHeight * ptrCurrentCamera->m_pixelsPerUnit * Screen::SCALE.Y);

			SDL_RenderSetClipRect((SDL_Renderer*)m_ptrRenderer, &clipRect); // m_renderer = your SDL_Renderer*
			

			/*ENGINE_INFO_D("Left Frame: " + std::to_string(leftFrame) + ", Top Frame : " + std::to_string(topFrame) + ", Right Frame : " + std::to_string(rightFrame) +
				", Bottom Frame: " + std::to_string(bottomFrame));*/

			// auto start = std::chrono::high_resolution_clock::now();

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

				bool isVisible = objectRight >= leftFrame && objectLeft <= rightFrame &&
					objectBottom >= topFrame && objectTop <= bottomFrame;

				if (isVisible)
				{
					// Draw the Sprite
					Draw(refTransform, m_refECS.GetComponent<Sprite>(refTransform.m_entity), ptrCurrentCamera->m_pixelsPerUnit, Vector2D<float>(leftOffset, topOffset));

					// If in debug, draw the transform
					//Draw(refTransform, ptrCurrentCamera->m_pixelsPerUnit, Vector2D<float>(leftOffset, topOffset));
				}
			}

			auto drawVisibleChains = [&](auto& components)
				{
					for (auto& refLineCollider : components)
					{
						float objectLeft = refLineCollider.m_points[1].X;
						float objectRight = refLineCollider.m_points[2].X;
						float objectTop = refLineCollider.m_points[1].Y;
						float objectBottom = refLineCollider.m_points[2].Y;

						bool isVisible = objectRight >= leftFrame && objectLeft <= rightFrame &&
							objectBottom >= topFrame && objectTop <= bottomFrame;

						if (isVisible)
						{
							Draw(refLineCollider, ptrCurrentCamera->m_pixelsPerUnit, Vector2D<float>(leftOffset, topOffset));
						}
					}
				};

			drawVisibleChains(m_refECS.GetHotComponents<ChainColliderLeft>());
			drawVisibleChains(m_refECS.GetHotComponents<ChainColliderRight>());
			drawVisibleChains(m_refECS.GetHotComponents<ChainColliderTop>());
			drawVisibleChains(m_refECS.GetHotComponents<ChainColliderBottom>());


			// If "border on"
			// Draw the camera frame
			SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

			SDL_RenderDrawLine((SDLRenderer*)m_ptrRenderer, clipRect.x, clipRect.y, clipRect.x + clipRect.w, clipRect.y); // top line
			SDL_RenderDrawLine((SDLRenderer*)m_ptrRenderer, clipRect.x, clipRect.y, clipRect.x, clipRect.y + clipRect.h); // left line
			SDL_RenderDrawLine((SDLRenderer*)m_ptrRenderer, clipRect.x + clipRect.w - 1, clipRect.y, clipRect.x + clipRect.w - 1, clipRect.y + clipRect.h); // right line
			SDL_RenderDrawLine((SDLRenderer*)m_ptrRenderer, clipRect.x, clipRect.y + clipRect.h - 1, clipRect.x + clipRect.w, clipRect.y + clipRect.h - 1); // bottom line
			

			// auto end = std::chrono::high_resolution_clock::now();
			// std::cout << "Rendering: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";
		}

		
		SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 64, 64, 64, SDL_ALPHA_OPAQUE);

		Display();
	}

	void WindowRenderer::Draw(Transform& transform, Sprite* sprite, const int pixelsPerUnit, const Vector2D<float> offset)
	{
		const float interpolation = Time::GetInterpolationFactor();

		float lerpedX = Lerp(transform.PrevPosition.X, transform.Position.X, interpolation);
		float lerpedY = Lerp(transform.PrevPosition.Y, transform.Position.Y, interpolation);

		const float scale = pixelsPerUnit * Screen::SCALE_CONSTANT;

		int width, height;
		if (sprite)
		{
			width = static_cast<int>(round(sprite->m_size.X * scale));
			height = static_cast<int>(round(sprite->m_size.Y * scale));
		}
		else
		{
			width = static_cast<int>(round(transform.Dimensions.X * scale));
			height = static_cast<int>(round(transform.Dimensions.Y * scale));
		}

		SDLRect dst
		{
			static_cast<int>(round((lerpedX - offset.X) * scale)),
			static_cast<int>(round((lerpedY - offset.Y) * scale)),
			width,
			height
		};

		SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 0, 0, 0, 0);
		SDL_RenderFillRect((SDLRenderer*)m_ptrRenderer, &dst);

		if (sprite)
		{
			SDLTexture* spriteTexture = (SDLTexture*)m_refAssetManager.GetTexture(sprite->m_entity);
			if (spriteTexture != nullptr)
			{
				SDLRect src
				{ 
					static_cast<int>(sprite->m_currentFrame * sprite->m_size.X), // x position in tiles.
					0, // y position in tiles.
					static_cast<int>(sprite->m_size.X), 
					static_cast<int>(sprite->m_size.Y) 
				};

				ISDL::RenderCopyEx((SDLRenderer*)m_ptrRenderer, spriteTexture, &src, &dst, transform.Rotation, nullptr, SDL_FLIP_NONE);
			}
		}
		else
		{
			ISDL::RenderCopyEx((SDLRenderer*)m_ptrRenderer, nullptr, nullptr, &dst, transform.Rotation, nullptr, SDL_FLIP_NONE);
		}
	}


	/*void WindowRenderer::Draw(Transform& refTransform, Sprite& refSprite, const int pixelsPerUnit, const Vector2D<float> offset)
	{
		const float interpolation = Time::GetInterpolationFactor();

		float lerpedX = Lerp(refTransform.PrevPosition.X, refTransform.Position.X, interpolation);
		float lerpedY = Lerp(refTransform.PrevPosition.Y, refTransform.Position.Y, interpolation);

		SDLRect dst
		{
			static_cast<int>(round((lerpedX - offset.X) * pixelsPerUnit * Screen::SCALE_CONSTANT)),
			static_cast<int>(round((lerpedY - offset.Y) * pixelsPerUnit * Screen::SCALE_CONSTANT)),
			static_cast<int>(round(refSprite.m_size * pixelsPerUnit * Screen::SCALE_CONSTANT)),
			static_cast<int>(round(refSprite.m_size * pixelsPerUnit * Screen::SCALE_CONSTANT))
		};

		SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 0, 0, 0, 0);
		SDL_RenderFillRect((SDLRenderer*)m_ptrRenderer, &dst);

		SDLTexture* spriteTexture = (SDLTexture*)m_refAssetManager.GetTexture(refSprite.m_entity);
		if (spriteTexture != nullptr)
		{
			ISDL::RenderCopyEx((SDLRenderer*)m_ptrRenderer, spriteTexture, nullptr, &dst, refTransform.Rotation, nullptr, SDL_FLIP_NONE);
		}
	}*/

	void WindowRenderer::Draw(Transform& transform, const int pixelsPerUnit, const Vector2D<float> offset)
	{
		const float interpolation = Time::GetInterpolationFactor();

		// The x, y, height, and width of the portion of the texture we want to render.
		SDLRect src = { 0, 0, 0, 0 };

		float lerpedX = Lerp(transform.PrevPosition.X, transform.Position.X, interpolation);
		float lerpedY = Lerp(transform.PrevPosition.Y, transform.Position.Y, interpolation);

		SDLRect dst
		{
			static_cast<int>(round((lerpedX - offset.X) * pixelsPerUnit * Screen::SCALE_CONSTANT)),
			static_cast<int>(round((lerpedY - offset.Y) * pixelsPerUnit * Screen::SCALE_CONSTANT)),
			static_cast<int>(round(transform.Dimensions.X * pixelsPerUnit * Screen::SCALE_CONSTANT)),
			static_cast<int>(round(transform.Dimensions.Y * pixelsPerUnit * Screen::SCALE_CONSTANT))
			
		};

		SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 0, 0, 0, 0);
		// SDL_RenderDrawRect((SDLRenderer*)m_ptrRenderer, &dst);
		SDL_RenderFillRect((SDLRenderer*)m_ptrRenderer, &dst);

		ISDL::RenderCopyEx((SDLRenderer*)m_ptrRenderer, nullptr, nullptr, &dst, transform.Rotation, nullptr, SDL_FLIP_NONE);

		// This should show the boundary of the physics body, not the texture.		
	}

	void WindowRenderer::Draw(ChainCollider& chainCollider, const int pixelsPerUnit, const Vector2D<float> offset)
	{
		SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

		for (int i = 0; i < 3; ++i)
		{
			SDL_RenderDrawLine((SDLRenderer*)m_ptrRenderer,
				static_cast<int>(round((chainCollider.m_points[i].X - offset.X) * pixelsPerUnit * Screen::SCALE_CONSTANT)),
				static_cast<int>(round((chainCollider.m_points[i].Y - offset.Y) * pixelsPerUnit * Screen::SCALE_CONSTANT)),
				static_cast<int>(round((chainCollider.m_points[i + 1].X - offset.X) * pixelsPerUnit * Screen::SCALE_CONSTANT)),
				static_cast<int>(round((chainCollider.m_points[i + 1].Y - offset.Y) * pixelsPerUnit * Screen::SCALE_CONSTANT))
			);
		}
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

	const float WindowRenderer::GetMonitorRefreshRate()
	{
		// Get the current display mode of the window to find out the refresh rate.
		SDLDisplayMode displayMode;
		if (ISDL::GetDesktopDisplayMode(0, &displayMode) != 0)
		{
			ENGINE_ERROR("Could not get display mode: " + std::string(ISDL::GetError()));
			return 60.0f; // Default to 60 if we can't get the refresh rate
		}

		// Return the refresh rate of the monitor
		return static_cast<float>(displayMode.refresh_rate);
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
