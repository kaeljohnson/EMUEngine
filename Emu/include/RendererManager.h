#pragma once

#include <memory>

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
        SDLRenderer* GetRenderer() const;

        ~RendererManager();

        const SDLTexture* LoadTexture(const char* filePath);
        void SetViewport();
        void ClearScreen();
        void RenderScene(std::shared_ptr<Scene> scene, const double interpolation, const float offsetX, const float offsetY);
        void Draw(SceneObject* sceneObject, const int pixelsPerMeter, const double interpolation, const float offsetX, const float offsetY);
        void Display();

        // Window related functions.
        SDLWindow* GetWindow() const;
        const int GetFullscreenWidth() const;
        const int GetFullscreenHeight() const;

        void ResizeWindow(const int newWindowWidth, const int newWindowHeight);
        void ToggleFullscreen();

        inline const float GetScaleX() const { return SCALE_X; }
        inline const float GetScaleY() const { return SCALE_Y; }

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

        int m_fullscreenWidth;
        int m_fullscreenHeight;

        SDLRenderer* m_ptrRenderer;

        const int VIRTUAL_WIDTH;
        const int VIRTUAL_HEIGHT;

        float SCALE_X;
        float SCALE_Y;
        float SCALE;

        int m_viewportWidth;
        int m_viewportHeight;
        int m_viewportX;
        int m_viewportY;

        bool m_rendererCreated;

        void free();
    };
}