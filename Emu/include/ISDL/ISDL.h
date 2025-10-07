#pragma once

#include "../Includes.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

// Wraps all SDL functions and types in case they change.

namespace Engine
{
    using SDLWindow = SDL_Window;
    using SDLRenderer = SDL_Renderer;
    using SDLRect = SDL_Rect;
    using SDLTexture = SDL_Texture;
    using SDLSurface = SDL_Surface;
    using SDLDisplayMode = SDL_DisplayMode;
    using SDLEvent = SDL_Event;

    class ISDL {
    public:
        static int Init() {
            return SDL_Init(SDL_INIT_VIDEO);
        }

        static int ImgInit(int flags) {
            return IMG_Init(flags);
        }

        static void Quit() {
            SDL_Quit();
        }

        static SDLWindow* CreateWindow(const char* title, int x, int y, int w, int h, Uint32 flags) {
            return SDL_CreateWindow(title, x, y, w, h, flags);
        }

        static void GetWindowSize(SDLWindow* window, int* width, int* height) {
            SDL_GetWindowSize(window, width, height);
        }

        static void RenderSetViewport(SDLRenderer* renderer, const SDLRect* viewport) {
            SDL_RenderSetViewport(renderer, viewport);
        }

        static SDLRenderer* CreateRenderer(SDLWindow* window, int index, Uint32 flags) {
            return SDL_CreateRenderer(window, index, flags);
        }

        static void DestroyWindow(SDLWindow* window) {
            SDL_DestroyWindow(window);
        }

        static int PollEvent(SDLEvent* event) {
            return SDL_PollEvent(event);
        }

        static int RenderClear(SDLRenderer* renderer) {
            return SDL_RenderClear(renderer);
        }

        static void RenderPresent(SDLRenderer* renderer) {
            SDL_RenderPresent(renderer);
        }

        static int RenderCopy(SDLRenderer* renderer, SDLTexture* texture, const SDLRect* src, const SDLRect* dest) {
            return SDL_RenderCopy(renderer, texture, src, dest);
        }

        static int RenderCopyEx(SDLRenderer* renderer, SDLTexture* texture, const SDL_Rect* src, const SDL_Rect* dest, double angle, const SDL_Point* center, SDL_RendererFlip flip) {
            return SDL_RenderCopyEx(renderer, texture, src, dest, angle, center, flip);
        }

        static void DestroyRenderer(SDLRenderer* renderer) {
            SDL_DestroyRenderer(renderer);
        }

        static SDLSurface* CreateRGBSurface(Uint32 flags, int width, int height, int depth, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask) {
            return SDL_CreateRGBSurface(flags, width, height, depth, Rmask, Gmask, Bmask, Amask);
        }

        static SDLTexture* CreateTextureFromSurface(SDLRenderer* renderer, SDLSurface* surface) {
            return SDL_CreateTextureFromSurface(renderer, surface);
        }

        static Uint32 MapRGB(const SDL_PixelFormat* format, Uint8 r, Uint8 g, Uint8 b) {
            return SDL_MapRGB(format, r, g, b);
        }

        static int SetRenderDrawColor(SDLRenderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
			return SDL_SetRenderDrawColor(renderer, r, g, b, a);
		}

        static int FillRect(SDLSurface* surface, const SDLRect* rect, Uint32 color) {
            return SDL_FillRect(surface, rect, color);
        }

        static int SetWindowFullscreen(SDLWindow* window, Uint32 flags) {
            return SDL_SetWindowFullscreen(window, flags);
        }

        static int GetDesktopDisplayMode(int displayIndex, SDLDisplayMode* displayMode) {
            return SDL_GetDesktopDisplayMode(displayIndex, displayMode);
        }

        static void SetWindowSize(SDLWindow* window, int width, int height) {
            SDL_SetWindowSize(window, width, height);
        }

        static Uint32 GetWindowFlags(SDLWindow* window) {
            return SDL_GetWindowFlags(window);
        }

        static void SetWindowPosition(SDLWindow* window, int x, int y) {
            SDL_SetWindowPosition(window, x, y);
        }

        static void FreeSurface(SDLSurface* surface) {
            SDL_FreeSurface(surface);
        }

        static void DestroyTexture(SDLTexture* texture) {
            SDL_DestroyTexture(texture);
        }

        static const char* GetError() {
            return SDL_GetError();
        }
    };
}
