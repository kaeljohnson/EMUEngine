#pragma once

#include "../include/SDLWrapper/SDLWrapper.h"

#include "Scenes/SceneObject.h"
#include "CallbackSystem/CallbackSystem.h"

namespace Engine
{
    class RendererManager
    {
    private:
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

        RendererManager();

    public:
        static RendererManager* GetInstance();
        void CreateRenderer(SDLWindow* window);
        SDLRenderer* GetRenderer() const;

        ~RendererManager();

        const SDLTexture* loadTexture(const char* filePath);
        void clearScreen();
        void render(SceneObject* sceneObject, const int pixelsPerMeter, const double interpolation);
        void setViewport(SDLWindow* ptrWindow);
        void display();
        void free();

        // Deleted functions to ensure our singleton instance cannot be copied or moved.
        RendererManager(const RendererManager&) = delete;
        RendererManager& operator=(const RendererManager&) = delete;
        RendererManager(RendererManager&&) = delete;
        RendererManager& operator=(RendererManager&&) = delete;
    };
}