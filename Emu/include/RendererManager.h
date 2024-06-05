#pragma once

#include "../include/SDLWrapper/SDLWrapper.h"

#include "Scenes/SceneObject.h"

namespace Engine
{
    class RendererManager
    {
    private:
        RendererManager();
        static RendererManager* instance;
        SDLRenderer* renderer;

        const int VIRTUAL_WIDTH;
        const int VIRTUAL_HEIGHT;

        float SCALE_X;
        float SCALE_Y;
        float SCALE;

        int viewportWidth;
        int viewportHeight;
        int viewportX;
        int viewportY;

        bool rendererCreated;

        void free();

    public:
        static RendererManager* GetInstance();
        void CreateRenderer(SDLWindow* window);
        SDLRenderer* GetRenderer() const;

        ~RendererManager();

        const SDLTexture* LoadTexture(const char* filePath);
        void SetViewport(SDLWindow* ptrWindow);
        void ClearScreen();
        void Render(SceneObject* sceneObject, const int pixelsPerMeter, const double interpolation);
        void Display();

        // Deleted functions to ensure our singleton instance cannot be copied or moved.
        RendererManager(const RendererManager&) = delete;
        RendererManager& operator=(const RendererManager&) = delete;
        RendererManager(RendererManager&&) = delete;
        RendererManager& operator=(RendererManager&&) = delete;
    };
}