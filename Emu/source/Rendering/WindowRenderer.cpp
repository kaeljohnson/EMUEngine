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
	Vector2D<int> Screen::DISPLAY_RESOLUTION = Vector2D<int>(0, 0);
	Vector2D<int> Screen::VIRTUAL_SIZE = Vector2D<int>(0, 0);

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

		Screen::DISPLAY_RESOLUTION.X = displayMode.w;
		Screen::DISPLAY_RESOLUTION.Y = displayMode.h;

		ENGINE_INFO_D("Display resolution: " + std::to_string(displayMode.w) + ", " + std::to_string(displayMode.h));

		SDL_SetWindowMinimumSize((SDL_Window*)m_ptrWindow, Screen::DISPLAY_RESOLUTION.X / 2, Screen::DISPLAY_RESOLUTION.Y / 2); // User should not be able to resize smaller than half the display resolution.

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

		if (m_refECS.GetHotComponents<Camera>().empty())
		{
			ENGINE_CRITICAL_D("No active cameras found.");
			throw std::runtime_error("No active cameras found.");
			return;
		}

		if (Screen::WINDOW_RESIZE_REQUEST)
		{
			SetViewport();
			Screen::WINDOW_RESIZE_REQUEST = false;
		}

		if (Screen::TOGGLE_FULLSCREEN_REQUEST)
		{
			ToggleFullscreen();
			SetViewport();
			Screen::TOGGLE_FULLSCREEN_REQUEST = false;
		}

		// Render cameras in order of array for now. Will change to render in order of Z index later.
		for (auto& refCamera : m_refECS.GetHotComponents<Camera>())
		{
			// ENGINE_CRITICAL_D("Number of objects: " + std::to_string(m_renderBucket[1].size()));

			SDL_Rect clipRect;
			clipRect.x = static_cast<int>(refCamera.m_position.X * Screen::VIEWPORT_SIZE.X);
			clipRect.y = static_cast<int>(refCamera.m_position.Y * Screen::VIEWPORT_SIZE.Y);
			clipRect.w = static_cast<int>(refCamera.m_frameDimensions.X * refCamera.m_pixelsPerUnit * Screen::SCALE.X);
			clipRect.h = static_cast<int>(refCamera.m_frameDimensions.Y * refCamera.m_pixelsPerUnit * Screen::SCALE.Y);

			SDL_RenderSetClipRect((SDL_Renderer*)m_ptrRenderer, &clipRect); // m_renderer = your SDL_Renderer*

			/*ENGINE_CRITICAL_D("Rect top left: (" + std::to_string(clipRect.x) + ", " + std::to_string(clipRect.y) + "), width: "
				+ std::to_string(clipRect.w) + ", height: " + std::to_string(clipRect.h));*/

			for (auto& [zIndex, renderObjects] : m_spriteRenderBucket)
			{
				for (auto& refRenderObject : renderObjects)
				{
					//ENGINE_CRITICAL_D("Rendering entity: " + std::to_string(refRenderObject.m_entity));
					// Draw the Render Object
					Draw(refRenderObject, refCamera.m_pixelsPerUnit);
				}
			}
			m_spriteRenderBucket.clear();

			/*auto drawVisibleChains = [&](auto& components)
				{
					for (auto& refLineCollider : components)  
					{
						Draw(refLineCollider, ptrCurrentCamera->m_pixelsPerUnit);
					}
				};*/

#ifndef NDEBUG
			for (auto& [zIndex, renderObjects] : m_debugRenderBucket)
			{
				for (auto& refDebugRect : renderObjects)
				{
					// Draw the Render Object
					DebugDraw(refDebugRect, refCamera.m_pixelsPerUnit);
				}
			}
			m_debugRenderBucket.clear();

			/*drawVisibleChains(m_refECS.GetHotComponents<ChainColliderLeft>());
			drawVisibleChains(m_refECS.GetHotComponents<ChainColliderRight>());
			drawVisibleChains(m_refECS.GetHotComponents<ChainColliderTop>());
			drawVisibleChains(m_refECS.GetHotComponents<ChainColliderBottom>());*/
#endif

			// If "border on"
			// Draw the camera frame
			//SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

			//SDL_RenderDrawLine((SDLRenderer*)m_ptrRenderer, clipRect.x, clipRect.y, clipRect.x + clipRect.w, clipRect.y); // top line
			//SDL_RenderDrawLine((SDLRenderer*)m_ptrRenderer, clipRect.x, clipRect.y, clipRect.x, clipRect.y + clipRect.h); // left line
			//SDL_RenderDrawLine((SDLRenderer*)m_ptrRenderer, clipRect.x + clipRect.w - 1, clipRect.y, clipRect.x + clipRect.w - 1, clipRect.y + clipRect.h); // right line
			//SDL_RenderDrawLine((SDLRenderer*)m_ptrRenderer, clipRect.x, clipRect.y + clipRect.h - 1, clipRect.x + clipRect.w, clipRect.y + clipRect.h - 1); // bottom line
			

			// auto end = std::chrono::high_resolution_clock::now();
			// std::cout << "Rendering: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";
		}

		
		SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 64, 64, 64, SDL_ALPHA_OPAQUE);

		Display();
	}	

	void WindowRenderer::DebugDraw(DebugRect& rect, const int m_pixelsPerUnit)
	{
		SDLRect dst
		{
			rect.m_locationInPixelsOnScreen.X,
			rect.m_locationInPixelsOnScreen.Y,
			rect.m_sizeInPixelsOnScreen.X,
			rect.m_sizeInPixelsOnScreen.Y
		};

		if (rect.m_filled)
		{
			SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderFillRect((SDLRenderer*)m_ptrRenderer, &dst);
			SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		}
		else
		{
			SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderDrawRect((SDLRenderer*)m_ptrRenderer, &dst);
			SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		}
	}

	void WindowRenderer::Draw(RenderObject& object, const int pixelsPerUnit)
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
		if (spriteTexture != nullptr)
		{
			// Draw the sprite
			ISDL::RenderCopyEx((SDLRenderer*)m_ptrRenderer, spriteTexture, &src, &dst, 0.0, nullptr, SDL_FLIP_NONE);
		}
		else
		{
			// black rectangle for entities without sprites
			SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 0, 0, 0, 0);
			SDL_RenderFillRect((SDLRenderer*)m_ptrRenderer, &dst);
			ISDL::RenderCopyEx((SDLRenderer*)m_ptrRenderer, nullptr, nullptr, &dst, 0.0, nullptr, SDL_FLIP_NONE);
		}
#ifndef NDEBUG
		// green boxes
		//SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
		//SDL_RenderDrawRect((SDLRenderer*)m_ptrRenderer, &dst);
		//SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

		//// transform border should also be submitted to render queue since render should not need to know about components.
		//Transform* ptrTransform = m_refECS.GetComponent<Transform>(object.m_entity);
		//float lerpedX = Lerp(ptrTransform->PrevPosition.X, ptrTransform->Position.X, Time::GetInterpolationFactor());
		//float lerpedY = Lerp(ptrTransform->PrevPosition.Y, ptrTransform->Position.Y, Time::GetInterpolationFactor());
		//const float scaleX = pixelsPerUnit * Screen::SCALE.X;
		//const float scaleY = pixelsPerUnit * Screen::SCALE.Y;

		//// draw the transform rectangle border
		//SDLRect transformRect
		//{
		//	static_cast<int>(round((lerpedX - cameraOffset.X) * scaleX)),
		//	static_cast<int>(round((lerpedY - cameraOffset.Y) * scaleY)),
		//	static_cast<int>(round(ptrTransform->Dimensions.X * pixelsPerUnit * Screen::SCALE_CONSTANT)),
		//	static_cast<int>(round(ptrTransform->Dimensions.Y * pixelsPerUnit * Screen::SCALE_CONSTANT))

		//};
		//// Red boxes
		//SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		//SDL_RenderDrawRect((SDLRenderer*)m_ptrRenderer, &transformRect);
		//SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
#endif


	}
	void WindowRenderer::Draw(Transform& transform, Animations* animations, const int pixelsPerUnit, const Vector2D<float> cameraOffset)
	{
		const float interpolation = Time::GetInterpolationFactor();

		float lerpedX = Lerp(transform.PrevPosition.X, transform.Position.X, interpolation);
		float lerpedY = Lerp(transform.PrevPosition.Y, transform.Position.Y, interpolation);

		const float scaleX = pixelsPerUnit * Screen::SCALE.X;
		const float scaleY = pixelsPerUnit * Screen::SCALE.Y;

		int width, height;
		float offsetFromTransformX = 0.0f;
		float offsetFromTransformY = 0.0f;
		if (animations)
		{
			const Animation& currentAnimation = animations->m_animations.at(animations->m_currentAnimation);

			width = static_cast<int>(round(currentAnimation.m_size.X * scaleX));
			height = static_cast<int>(round(currentAnimation.m_size.Y * scaleY));
			offsetFromTransformX = currentAnimation.m_offsetFromTransform.X;
			offsetFromTransformY = currentAnimation.m_offsetFromTransform.Y;
		}
		else
		{
			width = static_cast<int>(round(transform.Dimensions.X * scaleX));
			height = static_cast<int>(round(transform.Dimensions.Y * scaleY));
		}

		SDLRect dst
		{
			static_cast<int>(round((lerpedX - cameraOffset.X + offsetFromTransformX) * scaleX)),
			static_cast<int>(round((lerpedY - cameraOffset.Y + offsetFromTransformY) * scaleY)),
			width,
			height
		};

		if (animations)
		{
			const Animation& currentAnimation = animations->m_animations.at(animations->m_currentAnimation);

			SDLTexture* spriteTexture = (SDLTexture*)m_refAssetManager.GetTexture(animations->m_entity);
			if (spriteTexture != nullptr)
			{
				int x = static_cast<int>(currentAnimation.m_currentFrame % currentAnimation.m_dimensions.X * currentAnimation.m_pixelsPerFrame.X);
				int y = static_cast<int>(floor(currentAnimation.m_currentFrame / currentAnimation.m_dimensions.X) * currentAnimation.m_pixelsPerFrame.Y);

				SDLRect src
				{ 
					// What to muiltiple these by to get the correct size on the sprite sheet for any viewport?
					// Size is set by client per unit. 
					x, 
					y,
					static_cast<int>(currentAnimation.m_pixelsPerFrame.X), 
					static_cast<int>(currentAnimation.m_pixelsPerFrame.Y) 
				};

				// Draw the sprite
				ISDL::RenderCopyEx((SDLRenderer*)m_ptrRenderer, spriteTexture, &src, &dst, transform.Rotation, nullptr, SDL_FLIP_NONE);
			}
		}
		else
		{
			// black rectangle for entities without sprites
			SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 0, 0, 0, 0);
			SDL_RenderFillRect((SDLRenderer*)m_ptrRenderer, &dst);
			ISDL::RenderCopyEx((SDLRenderer*)m_ptrRenderer, nullptr, nullptr, &dst, transform.Rotation, nullptr, SDL_FLIP_NONE);
		}

#ifndef NDEBUG 

		// green boxes
		SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect((SDLRenderer*)m_ptrRenderer, &dst);
		SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

		// draw the transform rectangle border
		SDLRect transformRect
		{
			static_cast<int>(round((lerpedX - cameraOffset.X) * scaleX)),
			static_cast<int>(round((lerpedY - cameraOffset.Y) * scaleY)),
			static_cast<int>(round(transform.Dimensions.X * pixelsPerUnit * Screen::SCALE_CONSTANT)),
			static_cast<int>(round(transform.Dimensions.Y * pixelsPerUnit * Screen::SCALE_CONSTANT))

		};
		// Red boxes
		SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect((SDLRenderer*)m_ptrRenderer, &transformRect);
		SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
#endif
	}

	void WindowRenderer::Draw(ChainCollider& chainCollider, const int pixelsPerUnit, const Vector2D<float> offset)
	{
		// Draw the chain collider as a series of lines in red.
		SDL_SetRenderDrawColor((SDLRenderer*)m_ptrRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

		const float scaleX = pixelsPerUnit * Screen::SCALE.X;
		const float scaleY = pixelsPerUnit * Screen::SCALE.Y;

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

	void WindowRenderer::SetViewport()
	{
		int windowWidthInPixels, windowHeightInPixels;
		SDL_GetRendererOutputSize((SDLRenderer*)m_ptrRenderer, &windowWidthInPixels, &windowHeightInPixels);

		ENGINE_INFO_D("Window size in pixels: " + std::to_string(windowWidthInPixels) + ", " + std::to_string(windowHeightInPixels));

		float aspectRatio = (float)windowWidthInPixels / (float)windowHeightInPixels;   

		ENGINE_INFO_D("Aspect Ratio: " + std::to_string(aspectRatio));

		Screen::VIRTUAL_SIZE.Y = 720; // Fixed virtual height in units for consistent rendering.

		Screen::VIRTUAL_SIZE.X = Screen::VIRTUAL_SIZE.Y * aspectRatio; // Need the virtual width to be based on the fixed units of height. Otherwise will get weird stretching.

		const float scaleX = static_cast<float>(windowWidthInPixels) / Screen::VIRTUAL_SIZE.X;
		const float scaleY = static_cast<float>(windowHeightInPixels) / Screen::VIRTUAL_SIZE.Y;

		ENGINE_INFO_D("Scale X: " + std::to_string(scaleX) + ", Scale Y: " + std::to_string(scaleY));

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
			ISDL::SetWindowSize((SDLWindow*)m_ptrWindow, Screen::DISPLAY_RESOLUTION.X / 2, Screen::DISPLAY_RESOLUTION.Y / 2);
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
