#pragma once

#include <memory>

#include "../SDLWrapper/SDLWrapper.h"

#include "IRenderer.h"
#include "../Scenes/SceneObject.h"

namespace Engine
{
    class RendererManager
    {
    private:
        RendererManager();
        static RendererManager* instance;
        std::shared_ptr<IRenderer> m_renderer;

        void free();

    public:
        static RendererManager* GetInstance();
        void CreateRenderer(SDLWindow* window);
        std::shared_ptr<IRenderer> GetRenderer() const;

        ~RendererManager();

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