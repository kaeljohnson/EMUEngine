#pragma once

#include "../Includes.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

namespace Engine
{
    using SDLWindow = SDL_Window;
    using SDLRenderer = SDL_Renderer;
    using SDLRect = SDL_Rect;
    using SDLTexture = SDL_Texture;
    using SDLSurface = SDL_Surface;
    using SDLDisplayMode = SDL_DisplayMode;
    using SDLEvent = SDL_Event;

    /**
	* @brief A wrapper class for SDL functions to facilitate easier integration and usage within the engine.
    */
    class ISDL {
    public:
		/**
		* @brief Initializes the SDL library with video support.
        */
        static int Init() 
        {
            return SDL_Init(SDL_INIT_VIDEO);
        }

        /**
		* @brief Initializes the SDL_image library with the specified flags.
        */
        static int ImgInit(int flags) 
        {
            return IMG_Init(flags);
        }

		/**
		* @brief Initializes the SDL_mixer library with the specified flags.
		*/
		static int AudioInit(int flags) 
        {
			return Mix_Init(flags);
		}

		/**
		* @brief Opens the audio device with the specified parameters.
		*/
		static int MixOpenAudio(int frequency, Uint16 format, int channels, int chunksize) 
        {
			return Mix_OpenAudio(frequency, format, channels, chunksize);
		}

		/**
		* @brief Quits the SDL library, cleaning up all initialized subsystems.
		*/
        static void Quit() 
        {
            SDL_Quit();
        }

        /**
		* @brief Creates an SDL window with the specified parameters.
        * 
		* * @param title: The title of the window.
		* * @param x: The x position of the window.
		* * @param y: The y position of the window.
		* * @param w: The width of the window.
		* * @param h: The height of the window.
		* * @param flags: The window creation flags.
        */
        static SDLWindow* CreateWindow(const char* title, int x, int y, int w, int h, Uint32 flags) 
        {
            return SDL_CreateWindow(title, x, y, w, h, flags);
        }

        /**
		* @brief Retrieves the size of the specified SDL window.
        * 
		* * @param window: The SDL window.
		* * @param width: Pointer to store the width of the window.
		* * @param height: Pointer to store the height of the window.
        */
        static void GetWindowSize(SDLWindow* window, int* width, int* height) 
        {
            SDL_GetWindowSize(window, width, height);
        }

        /**
		* @brief Sets the viewport for the specified SDL renderer.
        * 
		* * @param renderer: The SDL renderer.
		* * @param viewport: The rectangle defining the viewport.
        */
        static void RenderSetViewport(SDLRenderer* renderer, const SDLRect* viewport) 
        {
            SDL_RenderSetViewport(renderer, viewport);
        }

        /**
		* @brief Creates an SDL renderer for the specified window with the given index and flags.
        * 
		* * @param window: The SDL window.
		* * @param index: The index of the rendering driver to initialize.
		* * @param flags: The renderer creation flags.
        */
        static SDLRenderer* CreateRenderer(SDLWindow* window, int index, Uint32 flags) 
        {
            return SDL_CreateRenderer(window, index, flags);
        }

        /**
		* @brief Destroys the specified SDL window.
        * 
		* * @param window: The SDL window to destroy.
        */
        static void DestroyWindow(SDLWindow* window) 
        {
            SDL_DestroyWindow(window);
        }

        /**
		* @brief Polls for currently pending events and retrieves them.
        * 
		* * @param event: Pointer to an SDL_Event structure to store the retrieved event.
        */
        static int PollEvent(SDLEvent* event) 
        {
            return SDL_PollEvent(event);
        }

		/**
		* @brief Clears the current rendering target with the drawing color.
        * 
		* * @param renderer: The SDL renderer.
		*/
        static int RenderClear(SDLRenderer* renderer) 
        {
            return SDL_RenderClear(renderer);
        }

        /**
		* @brief Updates the screen with any rendering performed since the previous call.
        * 
		* * @param renderer: The SDL renderer.
        */
        static void RenderPresent(SDLRenderer* renderer) 
        {
            SDL_RenderPresent(renderer);
        }

        /**
		* @brief Copies a portion of the texture to the current rendering target.
        * 
		* * @param renderer: The SDL renderer.
		* * @param texture: The SDL texture to copy from.
		* * @param src: The source rectangle (portion of the texture to copy).
		* * @param dest: The destination rectangle (where to copy on the rendering target).
        * 
		* * @return 0 on success, or a negative error code on failure.
        */
        static int RenderCopy(SDLRenderer* renderer, SDLTexture* texture, const SDLRect* src, const SDLRect* dest) 
        {
            return SDL_RenderCopy(renderer, texture, src, dest);
        }

        /**
		* @brief Copies a portion of the texture to the current rendering target with rotation and flipping.
        * 
		* * @param renderer: The SDL renderer.
		* * @param texture: The SDL texture to copy from.
		* * @param src: The source rectangle (portion of the texture to copy).
		* * @param dest: The destination rectangle (where to copy on the rendering target).
		* * @param angle: The angle of rotation in degrees.
		* * @param center: The point around which to rotate (NULL for the center of the dest rectangle).
		* * @param flip: The flipping mode (none, horizontal, vertical).
        * 
		* * @return 0 on success, or a negative error code on failure.
        */
        static int RenderCopyEx(SDLRenderer* renderer, SDLTexture* texture, const SDL_Rect* src, const SDL_Rect* dest, double angle, const SDL_Point* center, SDL_RendererFlip flip) 
        {
            return SDL_RenderCopyEx(renderer, texture, src, dest, angle, center, flip);
        }

        /**
		* @brief Destroys the specified SDL renderer.
        * 
		* * @param renderer: The SDL renderer to destroy.
        */
        static void DestroyRenderer(SDLRenderer* renderer) 
        {
            SDL_DestroyRenderer(renderer);
        }

        /**
		* @brief Creates an SDL surface with the specified parameters.
        * 
		* * @param flags: The surface creation flags.
		* * @param width: The width of the surface.
		* * @param height: The height of the surface.
		* * @param depth: The depth (bits per pixel) of the surface.
		* * @param Rmask: The red mask for the surface's pixel format.
		* * @param Gmask: The green mask for the surface's pixel format.
		* * @param Bmask: The blue mask for the surface's pixel format.
		* * @param Amask: The alpha mask for the surface's pixel format.
        * 
		* @return A pointer to the created SDL surface.
        */
        static SDLSurface* CreateRGBSurface(Uint32 flags, int width, int height, int depth, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask) 
        {
            return SDL_CreateRGBSurface(flags, width, height, depth, Rmask, Gmask, Bmask, Amask);
        }

		/**
		* @brief Creates an SDL texture from the given SDL surface.
        * 
		* * @param renderer: The SDL renderer.
		* * @param surface: The SDL surface to create the texture from.
		*
		* * @return A pointer to the created SDL texture.
		*/
        static SDLTexture* CreateTextureFromSurface(SDLRenderer* renderer, SDLSurface* surface) 
        {
            return SDL_CreateTextureFromSurface(renderer, surface);
        }

        /**
		* @brief Maps RGB color values to a pixel format.
        * 
		* * @param format: The SDL pixel format.
		* * @param r: The red color value.
		* * @param g: The green color value.
		* * @param b: The blue color value.
        * 
		* * @return The mapped pixel value.
        */
        static Uint32 MapRGB(const SDL_PixelFormat* format, Uint8 r, Uint8 g, Uint8 b) 
        {
            return SDL_MapRGB(format, r, g, b);
        }

        /**
		* @brief Sets the drawing color for the specified SDL renderer.
        * 
		* * @param renderer: The SDL renderer.
		* * @param r: The red color value.
		* * @param g: The green color value.
		* * @param b: The blue color value.
		* * @param a: The alpha (transparency) value.
        * 
		* * @return 0 on success, or a negative error code on failure.
        */
        static int SetRenderDrawColor(SDLRenderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a) 
        {
			return SDL_SetRenderDrawColor(renderer, r, g, b, a);
		}

        /**
		* @brief Fills a rectangle on the specified SDL surface with the given color.
        * 
		* * @param surface: The SDL surface.
		* * @param rect: The rectangle to fill.
		* * @param color: The color to fill the rectangle with.
        * 
		* * @return 0 on success, or a negative error code on failure.
        */
        static int FillRect(SDLSurface* surface, const SDLRect* rect, Uint32 color) 
        {
            return SDL_FillRect(surface, rect, color);
        }

        /**
		* @brief Sets the fullscreen mode for the specified SDL window.
        * 
		* * @param window: The SDL window.
		* * @param flags: The fullscreen flags.
        */
        static int SetWindowFullscreen(SDLWindow* window, Uint32 flags) 
        {
            return SDL_SetWindowFullscreen(window, flags);
        }

        /**
		* @brief Retrieves the current desktop display mode for the specified display index.
        * 
		* * @param displayIndex: The index of the display.
		* * @param displayMode: Pointer to an SDL_DisplayMode structure to store the retrieved display mode.
        * 
		* * @return 0 on success, or a negative error code on failure.
        */
        static int GetDesktopDisplayMode(int displayIndex, SDLDisplayMode* displayMode) 
        {
            return SDL_GetDesktopDisplayMode(displayIndex, displayMode);
        }

        /**
		* @brief Sets the size of the specified SDL window.
        * 
		* * @param window: The SDL window.
		* * @param width: The new width of the window.
		* * @param height: The new height of the window.
        */
        static void SetWindowSize(SDLWindow* window, int width, int height) {
            SDL_SetWindowSize(window, width, height);
        }

        /**
		* @brief Retrieves the flags of the specified SDL window.
        * 
		* * @param window: The SDL window.
		* * @return The window flags.
        */
        static Uint32 GetWindowFlags(SDLWindow* window) {
            return SDL_GetWindowFlags(window);
        }

		/**
		* @brief Sets the position of the specified SDL window.
        * 
		* * @param window: The SDL window.
		* * @param x: The new x position of the window.
		* * @param y: The new y position of the window.
        */
        static void SetWindowPosition(SDLWindow* window, int x, int y) 
        {
            SDL_SetWindowPosition(window, x, y);
        }

        /**
		* @brief Frees the specified SDL surface.
        * 
		* * @param surface: The SDL surface to free.
        */
        static void FreeSurface(SDLSurface* surface) 
        {
            SDL_FreeSurface(surface);
        }

		/**
		* @brief Destroys the specified SDL texture.
        * 
		* * @param texture: The SDL texture to destroy.
		*/
        static void DestroyTexture(SDLTexture* texture) 
        {
            SDL_DestroyTexture(texture);
        }

		/**
		* @brief Retrieves the last error message from SDL.
        * 
		* * @return A pointer to the error message string.
        */
        static const char* GetError() 
        {
            return SDL_GetError();
        }
    };
}
