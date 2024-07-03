#pragma once

#include <memory>

#include "../include/SDLWrapper/SDLWrapper.h"

#include "../include/RendererManager.h"
#include "../include/Logging/Logger.h"
#include "../include/Scenes/Scene.h"
#include "../include/Scenes/SceneObject.h"
#include "../include/Physics/IPhysicsBody.h"
#include "../include/CallbackSystem/CallbackSystem.h"
#include "../include/Textures/ITexture.h"
#include "../include/Textures/Texture.h"

namespace Engine
{
	// Initialize static member
	RendererManager* RendererManager::instance = nullptr;

	RendererManager::RendererManager() 
		: VIRTUAL_WIDTH(1280), VIRTUAL_HEIGHT(720), SCALE_X(0), SCALE_Y(0), SCALE(0), 
		m_viewportX(0), m_viewportY(0), m_viewportWidth(0), m_viewportHeight(0),
		m_screenWidth(0), m_screenHeight(0),
		m_rendererCreated(false), m_ptrWindow(nullptr), m_ptrRenderer(nullptr), 
		m_ptrCurrentScene(nullptr)
	{}

	void RendererManager::CreateRenderer()
	{
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

		m_screenWidth = displayMode.w;
		m_screenHeight = displayMode.h;


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
		m_rendererCreated ? ENGINE_INFO_D("Renderer is created.") : 
						  ENGINE_INFO_D("Renderer is not created. Returning nullptr.");

		return m_ptrRenderer;
	}

	SDLWindow* RendererManager::GetWindow() const
	{
		return m_ptrWindow;
	}

	const int RendererManager::GetFullscreenWidth() const
	{
		return m_screenWidth;
	}

	const int RendererManager::GetFullscreenHeight() const
	{
		return m_screenHeight;
	}
	
	void RendererManager::ToggleFullscreen()
	{
		// Bug here: Figure out why "SDL_WINDOW_FULLSCREEN" does not work.
		// Incompatibility with native video mode?
		bool isFullscreen = SDL_GET_WINDOW_FLAGS(m_ptrWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP;

		// Engine should not support resizing the simulation. That is, the pixels per meter should not change.
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
			SDL_SET_WINDOW_SIZE(m_ptrWindow, m_screenWidth / 2, m_screenHeight / 2);
			SDL_SET_WINDOW_POSITION(m_ptrWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		}
	}

	void RendererManager::ResizeWindow(const int newWindowWidth, const int newWindowHeight)
	{
		ENGINE_TRACE_D(std::to_string(newWindowWidth) + ", " + std::to_string(newWindowHeight));

		if (newWindowWidth < m_screenWidth / 2 && newWindowHeight < m_screenHeight / 2)
		{
			SDL_SET_WINDOW_SIZE(m_ptrWindow, m_screenWidth / 2, m_screenHeight / 2);
		}
		else if (newWindowWidth < m_screenWidth / 2)
		{
			SDL_SET_WINDOW_SIZE(m_ptrWindow, m_screenWidth / 2, newWindowHeight);
		}
		else if (newWindowHeight < m_screenHeight / 2)
		{
			SDL_SET_WINDOW_SIZE(m_ptrWindow, newWindowWidth, m_screenHeight / 2);
		}
		else
		{
			// Let OS handle resize.
		}

	}

	// Definition for loadTexture function in RendererManager. 
	// Returns a const SDL_Texture*. Takes a file (probably a sprite sheet) 
	// full of the textures we want to load.
	const SDLTexture* RendererManager::LoadTexture(const char* filePath)
	{
		SDLTexture* texture =  IMG_LoadTexture(m_ptrRenderer, filePath);
		if (texture == nullptr)
		{
			ENGINE_CRITICAL("Failed to load texture. SDL Error: " + std::string(SDL_GET_ERROR()));
		}
		return texture;
	}

	// Wrapper for SDL_RenderClear. Clears the screen.
	void RendererManager::ClearScreen()
	{
		SDL_RENDER_CLEAR(m_ptrRenderer);
	}

	// Wrapper for SDL_RenderPresent. Talks to the actual hardwares renderer to display the renderer.
	void RendererManager::Display()
	{
		SDL_RENDER_PRESENT(m_ptrRenderer);
	}

	void RendererManager::RenderScene(const double interpolation, const float cameraOffsetX, const float cameraOffsetY)
	{
		ClearScreen();

		// Calculate camera bounds
		float cameraLeft = cameraOffsetX;
		float cameraRight = cameraOffsetX + (m_viewportWidth / m_ptrCurrentScene->GetPixelsPerMeter());
		float cameraTop = cameraOffsetY;
		float cameraBottom = cameraOffsetY + (m_viewportHeight / m_ptrCurrentScene->GetPixelsPerMeter());

		for (auto& layer : m_ptrCurrentScene->GetLayers())
		{
			for (auto& sceneObject : layer)
			{
				float objectLeft = sceneObject->GetPhysicsBody()->GetTopLeftXInMeters();
				float objectRight = objectLeft + sceneObject->GetPhysicsBody()->GetWidthInMeters();
				float objectTop = sceneObject->GetPhysicsBody()->GetTopLeftYInMeters();
				float objectBottom = objectTop + sceneObject->GetPhysicsBody()->GetHeightInMeters();

				bool isVisible = objectRight >= cameraLeft && objectLeft <= cameraRight &&
					objectBottom >= cameraTop && objectTop <= cameraBottom;

				if (isVisible)
				{
					Draw(sceneObject, m_ptrCurrentScene->GetPixelsPerMeter(), interpolation, cameraLeft, cameraTop);
				}
			}
		}

		Display();
	}

	// Definition of render function for the RendererManager class. Takes a SDL_Rect reference which will be rendered.
	void RendererManager::Draw(SceneObject* sceneObject, const int pixelsPerMeter, const double interpolation, const float offsetX, const float offsetY)
	{
		bool isTextureNull = sceneObject->GetTexture() == nullptr;

		// The x, y, height, and width of the portion of the texture we want to render.
		SDLRect src = { 0, 0, 0, 0 };

		std::shared_ptr<IPhysicsBody> ptrBody = sceneObject->GetPhysicsBody();

		SDLRect dst
		{
			static_cast<int>(round(((ptrBody->GetTopLeftPrevX()) * (1.0 - interpolation) + (ptrBody->GetTopLeftXInMeters()) * interpolation - offsetX) * pixelsPerMeter * SCALE)),
			static_cast<int>(round(((ptrBody->GetTopLeftPrevY()) * (1.0 - interpolation) + (ptrBody->GetTopLeftYInMeters()) * interpolation - offsetY) * pixelsPerMeter * SCALE)),

			static_cast<int>(round(ptrBody->GetWidthInMeters() * pixelsPerMeter * SCALE)),
			static_cast<int>(round(ptrBody->GetHeightInMeters() * pixelsPerMeter * SCALE))
		};

		SDLTexture* ptrTexture = nullptr;

		if (!isTextureNull)
			ptrTexture = static_cast<Texture*>(sceneObject->GetTexture().get())->m_texture;

		SDL_RENDER_COPY_EX(m_ptrRenderer, ptrTexture, nullptr, &dst, ptrBody->GetAngleInDegrees(), nullptr, SDL_FLIP_NONE);

		// This should show the boundary of the physics body, not the texture.
#if defined(DEBUG)
		if (ptrBody->GetHasBottomCollision() || ptrBody->GetHasTopCollision() || 
			ptrBody->GetHasLeftCollision() || ptrBody->GetHasRightCollision())
		{
			SDL_SetRenderDrawColor(m_ptrRenderer, 0, 0, 255, 255);
		}
		else if (ptrBody->GetHasBottomSensor() || ptrBody->GetHasTopSensor() || 
				 ptrBody->GetHasLeftSensor() || ptrBody->GetHasRightSensor())
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

	void RendererManager::SetScene(std::shared_ptr<Scene> ptrScene)
	{
		m_ptrCurrentScene = ptrScene;
	}

	void RendererManager::SetViewport()
	{
		int windowWidth, windowHeight;
		SDL_GET_WINDOW_SIZE(m_ptrWindow, &windowWidth, &windowHeight);

		SCALE_X = static_cast<float>(windowWidth) / VIRTUAL_WIDTH;
		SCALE_Y = static_cast<float>(windowHeight) / VIRTUAL_HEIGHT;

		SCALE = std::min(SCALE_X, SCALE_Y);

		m_viewportWidth = static_cast<int>(VIRTUAL_WIDTH * SCALE);
		m_viewportHeight = static_cast<int>(VIRTUAL_HEIGHT * SCALE);

		m_viewportX = (windowWidth - m_viewportWidth) / 2;
		m_viewportY = (windowHeight - m_viewportHeight) / 2;

		SDLRect viewport = { m_viewportX, m_viewportY, m_viewportWidth, m_viewportHeight };
		SDL_RENDER_SET_VIEWPORT(m_ptrRenderer, &viewport);
	}

	void RendererManager::free()
	{
		ENGINE_INFO("Freeing Renderer.");
		SDL_DESTROY_RENDERER(m_ptrRenderer);
		m_ptrRenderer = nullptr;

		ENGINE_INFO("Freeing Window.");
		SDL_DESTROY_WINDOW(m_ptrWindow);
		m_ptrWindow = nullptr;
	}

	// Definition for the RendererManager destructor. Calls the free function.
	RendererManager::~RendererManager()
	{
		free();
	}
}