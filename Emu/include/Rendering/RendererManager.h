#pragma once

#include <memory>

#include "../MathUtil.h"

#include "../../include/Scenes/Scene.h"
#include "../Scenes/SceneObject.h"

namespace Engine
{
    class WindowRenderer;

    class RendererManager
    {
    public:
        RendererManager();
        ~RendererManager();

        void SetScene(std::shared_ptr<Scene> scene);
        void RenderScene(const double interpolation, const Vector2D<float> offset);

        // Window related functions.
        const int GetFullscreenWidth() const;
        const int GetFullscreenHeight() const;

        void SetViewport();
        void ResizeWindow(const int newWindowWidth, const int newWindowHeight);
        void ToggleFullscreen();

        const Vector2D<float> GetScale() const;

        // Deleted functions to ensure our singleton instance cannot be copied or moved.
        RendererManager(const RendererManager&) = delete;
        RendererManager& operator=(const RendererManager&) = delete;
        RendererManager(RendererManager&&) = delete;
        RendererManager& operator=(RendererManager&&) = delete;

    private:
        std::shared_ptr<Scene> m_ptrCurrentScene;

	private:
        std::unique_ptr<WindowRenderer> m_ptrWindowRenderer;

        void free();
    };
}