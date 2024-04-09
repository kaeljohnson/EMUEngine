#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace Engine
{
	using SDLWindow = SDL_Window;
	using SDLRenderer = SDL_Renderer;
	using SDLRect = SDL_Rect;
	using SDLTexture = SDL_Texture;
	using SDLSurface = SDL_Surface;
	using SDLDisplayMode = SDL_DisplayMode;
	using SDLEvent = SDL_Event;

	#define SDL_INIT() SDL_Init(SDL_INIT_VIDEO)
	#define IMG_INIT(flags) IMG_Init(flags)
	#define SDL_QUIT() SDL_Quit()
	#define SDL_CREATE_WINDOW(title, x, y, w, h, flags) SDL_CreateWindow(title, x, y, w, h, flags)
	#define SDL_GET_WINDOW_SIZE(ptrWindow, ptrWidth, ptrHeight) SDL_GetWindowSize(ptrWindow, ptrWidth, ptrHeight);
	#define SDL_RENDER_SET_VIEWPORT(ptrRenderer, ptrViewport) SDL_RenderSetViewport(ptrRenderer, ptrViewport);
	#define SDL_CREATE_RENDERER(window, index, flags) SDL_CreateRenderer(window, index, flags)
	#define SDL_DESTROY_WINDOW(window) SDL_DestroyWindow(window)
	#define SDL_POLL_EVENT(event) SDL_PollEvent(event)
	#define SDL_RENDER_CLEAR(renderer) SDL_RenderClear(renderer)
	#define SDL_RENDER_PRESENT(renderer) SDL_RenderPresent(renderer)
	#define SDL_RENDER_COPY(renderer, texture, src, dest) SDL_RenderCopy(renderer, texture, src, dest)
	#define SDL_RENDER_COPY_EX(renderer, texture, src, dest, angle, center, flip) SDL_RenderCopyEx(renderer, texture, src, dest, angle, center, flip)
	#define SDL_DESTROY_RENDERER(renderer) SDL_DestroyRenderer(renderer)
	#define SDL_CREATE_RGB_SURFACE(flags, width, height, depth, Rmask, Gmask, Bmask, Amask) SDL_CreateRGBSurface(flags, width, height, depth, Rmask, Gmask, Bmask, Amask)
	#define SDL_CREATE_TEXTURE_FROM_SURFACE(renderer, surface) SDL_CreateTextureFromSurface(renderer, surface)
	#define SDL_MAP_RGB(format, r, g, b) SDL_MapRGB(format, r, g, b)
	#define SDL_FILL_RECT(surface, rect, color) SDL_FillRect(surface, rect, color)
	#define SDL_SET_WINDOW_FULLSCREEN(window, flags) SDL_SetWindowFullscreen(window, flags)
	#define SDL_GET_DESKTOP_DISPLAY_MODE(displayIndex, displayMode) SDL_GetDesktopDisplayMode(displayIndex, displayMode)
	#define SDL_SET_WINDOW_SIZE(window, width, height) SDL_SetWindowSize(window, width, height)
	#define SDL_GET_WINDOW_FLAGS(window) SDL_GetWindowFlags(window)
	#define SDL_SET_WINDOW_POSITION(window, x, y) SDL_SetWindowPosition(window, x, y)
	#define SDL_FREE_SURFACE(surface) SDL_FreeSurface(surface)
	#define SDL_DESTROY_TEXTURE(texture) SDL_DestroyTexture(texture)
	#define SDL_GET_ERROR() SDL_GetError()

}
