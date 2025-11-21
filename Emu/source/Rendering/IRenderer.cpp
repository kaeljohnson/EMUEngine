#pragma once

#include "../../include/ISDL/ISDL.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Rendering/IRenderer.h"
#include "../../include/Camera/CameraInterface.h"
#include "../../include/ECS/ECS.h"
#include "../../include/Components.h"
#include "../../include/Time.h"
#include "../../include/Rendering/Screen.h"

#include <chrono>

namespace Engine
{
	bool Screen::WINDOW_RESIZE_REQUEST = false;
	bool Screen::TOGGLE_FULLSCREEN_REQUEST = false;
	Vector2D<int> Screen::VIEWPORT_SIZE = Vector2D<int>(0, 0);
	Vector2D<int> Screen::VIEWPORT_POSITION = Vector2D<int>(0, 0);
	Vector2D<float> Screen::SCALE = Vector2D<float>(0.0f, 0.0f);
	float Screen::SCALE_CONSTANT = 0.0f;
	Vector2D<int> Screen::DISPLAY_RESOLUTION = Vector2D<int>(0, 0);
	Vector2D<int> Screen::VIRTUAL_SIZE = Vector2D<int>(0, 0);

	IRenderer::IRenderer(ECS& refECS, AssetManager& refAssetManager) 
		: m_rendererCreated(false), m_ptrWindow(nullptr), m_ptrRenderer(nullptr), m_refECS(refECS), m_refAssetManager(refAssetManager), m_lastDebugColor(DebugColor::Black)
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
			ENGINE_CRITICAL("Window not created! SDL_Error: {}", ISDL::GetError());
		}

		ENGINE_INFO_D("Window created!");

		SDLDisplayMode displayMode;
		if (ISDL::GetDesktopDisplayMode(0, &displayMode) != 0)
		{
			ENGINE_CRITICAL("Get desktop display mode failed: {}", ISDL::GetError());
		}

		Screen::DISPLAY_RESOLUTION.X = displayMode.w;
		Screen::DISPLAY_RESOLUTION.Y = displayMode.h;

		ENGINE_INFO_D("Display resolution: {}, {}", displayMode.w, displayMode.h);

		SDL_SetWindowMinimumSize((SDL_Window*)m_ptrWindow, Screen::DISPLAY_RESOLUTION.X / 2, Screen::DISPLAY_RESOLUTION.Y / 2); // User should not be able to resize smaller than half the display resolution.

		// Create renderer
		m_ptrRenderer = ISDL::CreateRenderer((SDLWindow*)m_ptrWindow, -1, SDL_RENDERER_ACCELERATED);
		if (m_ptrRenderer == nullptr)
		{
			ENGINE_CRITICAL("Renderer could not be created! SDL Error: {}", ISDL::GetError());
		}

		ISDL::SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 64, 64, 64, SDL_ALPHA_OPAQUE);

		setViewport();

		// Renderer only supports one window.
		m_rendererCreated = true;

		// Give renderer pointer to asset manager so it can load textures.
		m_refAssetManager.GiveRenderer(m_ptrRenderer);

		ENGINE_CRITICAL("Renderer created.");
	}

	void IRenderer::CheckForWindowResizeRequest()
	{
		if (Screen::WINDOW_RESIZE_REQUEST)
		{
			setViewport();
			Screen::WINDOW_RESIZE_REQUEST = false;
		}

		if (Screen::TOGGLE_FULLSCREEN_REQUEST)
		{
			ToggleFullscreen();
			setViewport();
			Screen::TOGGLE_FULLSCREEN_REQUEST = false;
		}
	}

	void IRenderer::Render()
	{
		// auto startTime = std::chrono::high_resolution_clock::now();

		clearScreen();

		for (auto& refCamera : m_refECS.GetHotComponents<Camera>())
		{
			SDL_Rect clipRect;
			clipRect.x = refCamera.m_clipRectPosition.X;
			clipRect.y = refCamera.m_clipRectPosition.Y;
			clipRect.w = refCamera.m_clipRectSize.X;
			clipRect.h = refCamera.m_clipRectSize.Y;

			SDL_RenderSetClipRect((SDL_Renderer*)m_ptrRenderer, &clipRect);

			for (auto ptrToVec = refCamera.m_renderBucket.rbegin(); ptrToVec != refCamera.m_renderBucket.rend(); ++ptrToVec)
			{
				for (auto& value : *ptrToVec)
				{
					draw(value);
				}

				ptrToVec->clear();
			}

#ifndef NDEBUG // DO NOT ADD DEBUG OBJECTS WHEN NOT IN DEBUG AS THE QUEUES WILL GROW INDEFINITELY.
			for (auto ptrToVec = refCamera.m_debugRenderBucket.rbegin(); ptrToVec != refCamera.m_debugRenderBucket.rend(); ++ptrToVec)
			{
				for (auto& value : *ptrToVec)
				{
					draw(value);
				}

				ptrToVec->clear();
			}


			for (auto ptrToVec = refCamera.m_debugLinesRenderBucket.rbegin(); ptrToVec != refCamera.m_debugLinesRenderBucket.rend(); ++ptrToVec)
			{
				for (auto& value : *ptrToVec)
				{
					draw(value);
				}

				ptrToVec->clear();
			}
#endif
			// Reset background color. Temp for now.
			SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 173, 216, 230, SDL_ALPHA_OPAQUE);

			if (refCamera.m_borderOn)
			{
				SDL_RenderDrawLine((SDLRenderer*)m_ptrRenderer, clipRect.x, clipRect.y, clipRect.x + clipRect.w, clipRect.y); // top line
				SDL_RenderDrawLine((SDLRenderer*)m_ptrRenderer, clipRect.x, clipRect.y, clipRect.x, clipRect.y + clipRect.h); // left line
				SDL_RenderDrawLine((SDLRenderer*)m_ptrRenderer, clipRect.x + clipRect.w - 1, clipRect.y, clipRect.x + clipRect.w - 1, clipRect.y + clipRect.h); // right line
				SDL_RenderDrawLine((SDLRenderer*)m_ptrRenderer, clipRect.x, clipRect.y + clipRect.h - 1, clipRect.x + clipRect.w, clipRect.y + clipRect.h - 1); // bottom line
			}
		}

		display();

		/*auto endTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float, std::milli> renderDuration = endTime - startTime;
		ENGINE_INFO_D("Render time (ms): " + std::to_string(renderDuration.count()));*/
	}	

	void IRenderer::draw(RenderObject& object)
	{
		SDLRect dst
		{
			object.m_locationInPixelsOnScreen.X,
			object.m_locationInPixelsOnScreen.Y,
			object.m_sizeInPixelsOnScreen.X,
			object.m_sizeInPixelsOnScreen.Y
		};

		SDLRect src
		{
			object.m_locationInPixelsOnSpriteSheet.X,
			object.m_locationInPixelsOnSpriteSheet.Y,
			object.m_sizeInPixelsOnSpriteSheet.X,
			object.m_sizeInPixelsOnSpriteSheet.Y
		};


		SDLTexture* spriteTexture = (SDLTexture*)m_refAssetManager.GetTexture(object.m_entity);

		// Maybe don't need the check here even though its safe?
		if (spriteTexture != nullptr)
		{
			// Draw the sprite
			ISDL::RenderCopyEx((SDLRenderer*)m_ptrRenderer, spriteTexture, &src, &dst, 0.0, nullptr, SDL_FLIP_NONE);
		}
		else
		{
			throw std::runtime_error("Entity with no texture in wrong queue. " + std::to_string(object.m_entity));
		}
	}

	void IRenderer::draw(DebugObject& object)
	{
		SDLRect dst
		{
			object.m_locationInPixelsOnScreen.X,
			object.m_locationInPixelsOnScreen.Y,
			object.m_sizeInPixelsOnScreen.X,
			object.m_sizeInPixelsOnScreen.Y
		};

		DebugColor debugColor = object.m_debugColor;

		// Only change debug color if different from last time to reduce calls to GPU.
		// if (debugColor != m_lastDebugColor) TODO: this is causing a bug where it the if block is never entered. 
		{
			switch (debugColor)
			{
			case DebugColor::Green:
				SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
				break;
			case DebugColor::Blue:
				SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
				break;
			case DebugColor::Black:
				SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
				break;
			default: // Red
				SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
				break;
			}
			m_lastDebugColor = debugColor;
		}

		if (object.m_filled)
		{
			SDL_RenderFillRect((SDLRenderer*)m_ptrRenderer, &dst);
		}
		else
		{
			SDL_RenderDrawRect((SDLRenderer*)m_ptrRenderer, &dst);
		}
	}

	void IRenderer::draw(LineObject& lineObject)
	{
		switch (lineObject.m_debugColor)
		{
		case DebugColor::Green:
			SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
			break;
		case DebugColor::Blue:
			SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
			break;
		case DebugColor::Black:
			SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
			break;
		default: // Red
			SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
			break;
		}

		SDL_RenderDrawLine((SDLRenderer*)m_ptrRenderer, lineObject.m_endPointInPixelsOnScreen.X, lineObject.m_endPointInPixelsOnScreen.Y,
			lineObject.m_startPointInPixelsOnScreen.X, lineObject.m_startPointInPixelsOnScreen.Y);
		
	}

	// Wrapper for SDL_RenderPresent. Talks to the actual hardwares renderer to display the renderer.
	void IRenderer::display()
	{
		ISDL::RenderPresent((SDLRenderer*)m_ptrRenderer);
	}

	// Wrapper for SDL_RenderClear. Clears the screen.
	void IRenderer::clearScreen()
	{
		ISDL::RenderClear((SDLRenderer*)m_ptrRenderer);
	}

	void IRenderer::setViewport()
	{
		int windowWidthInPixels, windowHeightInPixels;
		SDL_GetRendererOutputSize((SDLRenderer*)m_ptrRenderer, &windowWidthInPixels, &windowHeightInPixels);

		ENGINE_INFO_D("Window size in pixels: {}, {}", windowWidthInPixels, windowHeightInPixels);

		float aspectRatio = (float)windowWidthInPixels / (float)windowHeightInPixels;   

		ENGINE_INFO_D("Aspect Ratio: {}", aspectRatio);

		Screen::VIRTUAL_SIZE.Y = 720; // Fixed virtual height in units for consistent rendering.

		Screen::VIRTUAL_SIZE.X = Screen::VIRTUAL_SIZE.Y * aspectRatio; // Need the virtual width to be based on the fixed units of height. Otherwise will get weird stretching.

		const float scaleX = static_cast<float>(windowWidthInPixels) / Screen::VIRTUAL_SIZE.X;
		const float scaleY = static_cast<float>(windowHeightInPixels) / Screen::VIRTUAL_SIZE.Y;

		ENGINE_INFO_D("Scale X: {}, Scale Y: {}", scaleX, scaleY);

		Screen::SCALE = Vector2D<float>(scaleX, scaleY);

		Screen::SCALE_CONSTANT = scaleY; // Use height as fixed scale factor since most monitors are wider than they are tall. This also allows for variable display width with no distortion.

		const int viewportWidth = static_cast<int>(Screen::VIRTUAL_SIZE.X * Screen::SCALE_CONSTANT);
		const int viewportHeight = static_cast<int>(Screen::VIRTUAL_SIZE.Y * Screen::SCALE_CONSTANT);

		Screen::VIEWPORT_SIZE = Vector2D<int>(viewportWidth, viewportHeight);

		Screen::VIEWPORT_POSITION.X = (windowWidthInPixels - Screen::VIEWPORT_SIZE.X) / 2;
		Screen::VIEWPORT_POSITION.Y = (windowHeightInPixels - Screen::VIEWPORT_SIZE.Y) / 2;

		SDLRect viewport = { Screen::VIEWPORT_POSITION.X, Screen::VIEWPORT_POSITION.Y, Screen::VIEWPORT_SIZE.X, Screen::VIEWPORT_SIZE.Y };
		ISDL::RenderSetViewport((SDLRenderer*)m_ptrRenderer, &viewport);
	}

	void IRenderer::ToggleFullscreen()
	{
		// Bug here: Figure out why "SDL_WINDOW_FULLSCREEN" does not work.
		// Incompatibility with native video mode?
		bool isFullscreen = ISDL::GetWindowFlags((SDLWindow*)m_ptrWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP;

		// Renderer should not support resizing the simulation. That is, the pixels per unit should not change.
		// What will happen when the window size changes is that the camera will center on the player, or 
		// whatever object the camera is locked onto, until it hits the edge of the screen.

		if (ISDL::SetWindowFullscreen((SDLWindow*)m_ptrWindow, isFullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP) < 0)
		{
			ENGINE_ERROR("Fullscreen failed! SDL_Error: {}", ISDL::GetError());
		}
		else
		{
			// Default behavior for now will be to toggle fullscreen on for client.
			// When the screen is toggled to windowed, the size will be half of the width and height.
			ISDL::SetWindowSize((SDLWindow*)m_ptrWindow, Screen::DISPLAY_RESOLUTION.X / 2, Screen::DISPLAY_RESOLUTION.Y / 2);
			ISDL::SetWindowPosition((SDLWindow*)m_ptrWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		}
	}

	const float IRenderer::GetMonitorRefreshRate()
	{
		// Get the current display mode of the window to find out the refresh rate.
		SDLDisplayMode displayMode;
		if (ISDL::GetDesktopDisplayMode(0, &displayMode) != 0)
		{
			ENGINE_ERROR("Could not get display mode: {}", ISDL::GetError());
			return 60.0f; // Default to 60 if we can't get the refresh rate
		}

		// Return the refresh rate of the monitor
		return static_cast<float>(displayMode.refresh_rate);
	}

	void IRenderer::free()
	{
		ENGINE_INFO("Freeing Renderer.");
		ISDL::DestroyRenderer((SDLRenderer*)m_ptrRenderer);
		m_ptrRenderer = nullptr;

		ENGINE_INFO("Freeing Window.");
		ISDL::DestroyWindow((SDLWindow*)m_ptrWindow);
		m_ptrWindow = nullptr;
	}

	IRenderer::~IRenderer()
	{
	}
}
