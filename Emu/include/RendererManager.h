#pragma once

#include <memory>

#include "../include/MathUtil.h"

#include "../include/SDLWrapper/SDLWrapper.h"

#include "../include/Scenes/Scene.h"
#include "Scenes/SceneObject.h"

namespace Engine
{
    class RendererManager
    {
    public:
        static RendererManager* GetInstance();
        void CreateRenderer();

        ~RendererManager();

        void SetScene(std::shared_ptr<Scene> scene);

        
        void SetViewport();
        void ClearScreen();
        void RenderScene(const double interpolation, const Vector2D<float> offset);
        void Draw(SceneObject* sceneObject, const int pixelsPerMeter, const double interpolation, const Vector2D<float> offset);
        void Display();

        // Window related functions.
        
        inline const int GetFullscreenWidth() const { return m_screenSize.X; }
        const int GetFullscreenHeight() const { return m_screenSize.Y; }

        void ResizeWindow(const int newWindowWidth, const int newWindowHeight);
        void ToggleFullscreen();

        // inline const float GetScaleX() const { return SCALE_X; }
        // inline const float GetScaleY() const { return SCALE_Y; }

        inline const Vector2D<float> GetScale() const { return m_scale; }

        // Deleted functions to ensure our singleton instance cannot be copied or moved.
        RendererManager(const RendererManager&) = delete;
        RendererManager& operator=(const RendererManager&) = delete;
        RendererManager(RendererManager&&) = delete;
        RendererManager& operator=(RendererManager&&) = delete;

    private:
        RendererManager();
        static RendererManager* instance;

        // May need to decouple if there is ever a need for 
        // the application to have multiple windows.
        SDLWindow* m_ptrWindow;

        SDLRenderer* m_ptrRenderer;

        const SDLTexture* LoadTexture(const char* filePath);

        SDLWindow* GetWindow() const;

        SDLRenderer* GetRenderer() const;

        std::shared_ptr<Scene> m_ptrCurrentScene;

	private:
        // int m_screenWidth;
        // int m_screenHeight;
        Vector2D<int> m_screenSize;

        // const int VIRTUAL_WIDTH;
        // const int VIRTUAL_HEIGHT;
        Vector2D<int> m_virtualSize;

        Vector2D<float> m_scale;
        float SCALE;

        // int m_viewportWidth;
        // int m_viewportHeight;
        Vector2D<int> m_viewportSize;

        // int m_viewportX;
        // int m_viewportY;
        Vector2D<int> m_viewportPosition;

        bool m_rendererCreated;

        void free();
    };
}