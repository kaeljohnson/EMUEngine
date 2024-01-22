#pragma once																		// Ensures there are no units compiled more than once.

#include <SDL.h>																	// External library includes.
#include <stdio.h>

#include "../include/Application.h"													// Header files includes.
#include "../include/RendererManager.h"												//////////////////////
#include "../include/Events/EventManager.h"
#include "../include/Events/EventHandlers.h"

namespace Engine
{

	Application::Application(const char* appName, const uint16_t screenWidth, const uint16_t screenHeight)										// Definition for Game constructor. Note: "<Class>::<Function>" the function on the right side of the "::" comes from the class on the left side.
		: m_windowManager(appName, screenWidth, screenHeight),
		  m_rendererManager(m_windowManager.getWindow()),
		  m_eventManager(),
		  m_eventHandlers(),
		  running(false)																							// Initialization list for the Game constructor. Initialization lists are more optimal than setting variables within the square brackets of the constructor.
	{}

	void Application::run()																	// Definition for The Game class start function.
	{
		SDL_Event e;
		running = true;																	// Once the game is started, the running variable is set to "true" so the game loop runs indefinitely.
		 
		while (running)																	// Game loop.
		{
			while (SDL_PollEvent(&e) != 0)
			{
				dispatchEvent(e);
			}

			m_rendererManager.clearScreen();											// Call to clear screen function on RendererManager object reference. Screen must be cleared once a game loop.
			// m_rendererManager.render(/* Rect */);									// Here is where we will need to render all of our game objects: Player, entities, tiles, etc.
			m_rendererManager.display();												// Call to display function on RendererManager object reference. Once everything is rendered, we must display it on the screen.
		}
	}

	void Application::end()																// Definition for end function on game. Stops the game.
	{
		running = false;
		m_rendererManager.free();
		m_windowManager.free();
	}

	void Application::dispatchEvent(SDL_Event& e)
	{
		// Need to put all the following code into own modules.
		switch (e.type)
		{
		case (SDL_QUIT):
		{
			// Call quit event handler.
			m_eventHandlers.quit();
 			end();
			break;
		}
		case (SDL_WINDOWEVENT):
		{
			// Call window event handler.
			dispatchWindowEvent(e.window);
			break;
		}
		case (SDL_KEYDOWN):
		{
			// Call keydown event handler.
			// m_eventHandlers.keydown();
			dispatchKeydownEvent(e.key.keysym.sym);
			break;
		}
		case (SDL_KEYUP):
		{
			// Call key up event handler.
			dispatchKeyupEvent(e.key.keysym.sym);
			break;
		}
		case (SDL_MOUSEBUTTONDOWN):
		{
			// Call mouse button down event handler.
			dispatchMouseButtonDownEvent(e.button);
			break;
		}
		case (SDL_MOUSEBUTTONUP):
		{
			// Call mouse button up event handler.
			dispatchMouseButtonUpEvent(e.button);
			break;
		}
		case (SDL_MOUSEMOTION):
		{
			// Call mouse move event handler.
			dispatchMouseMoveEvent(e.motion);
			break;
		}
		case (SDL_MOUSEWHEEL):
		{
			// Call mouse wheel event handler.
			dispatchMouseScrollEvent(e.wheel);
			break;
		}
		default:
		{
			if (e.key.keysym.sym == SDLK_ESCAPE)
			{
				printf("end2");
				// bug here: This function is getting called seemingly randomly.
				//end();
			}
			break;
		}
		}
	}

	void Application::dispatchWindowEvent(SDL_WindowEvent& windowEvent)
	{
		switch (windowEvent.type)
		{
		case (SDL_WINDOWEVENT_RESIZED): m_windowManager.resize(windowEvent.data1, windowEvent.data2); break;
		}
	}

	void Application::dispatchKeydownEvent(SDL_Keycode& keyCode)
	{
		printf("dispatch keydown event.\n");
		switch (keyCode)
		{
		case (SDLK_ESCAPE): printf("esc down\n"); break;
		case (SDLK_EQUALS): m_windowManager.toggleFullscreen(); break;
		case (SDLK_0): printf("0 down\n"); break;
		case (SDLK_1): printf("1 down\n"); break;
		case (SDLK_2): printf("2 down\n"); break;
		case (SDLK_3): printf("3 down\n"); break;
		case (SDLK_4): printf("4 down\n"); break;
		case (SDLK_5): printf("5 down\n"); break;
		case (SDLK_6): printf("6 down\n"); break;
		case (SDLK_7): printf("7 down\n"); break;
		case (SDLK_8): printf("8 down\n"); break;
		case (SDLK_9): printf("9 down\n"); break;
		case (SDLK_q): printf("q down\n"); break;
		case (SDLK_w): printf("w down\n"); break;
		case (SDLK_e): printf("e down\n"); break;
		case (SDLK_r): printf("r down\n"); break;
		case (SDLK_t): printf("t down\n"); break;
		case (SDLK_y): printf("y down\n"); break;
		case (SDLK_u): printf("u down\n"); break;
		case (SDLK_i): printf("i down\n"); break;
		case (SDLK_o): printf("o down\n"); break;
		case (SDLK_p): printf("p down\n"); break;
		case (SDLK_a): printf("a down\n"); break;
		case (SDLK_s): printf("s down\n"); break;
		case (SDLK_d): printf("d down\n"); break;
		case (SDLK_f): printf("f down\n"); break;
		case (SDLK_g): printf("g down\n"); break;
		case (SDLK_h): printf("h down\n"); break;
		case (SDLK_j): printf("j down\n"); break;
		case (SDLK_k): printf("k down\n"); break;
		case (SDLK_l): printf("l down\n"); break;
		case (SDLK_z): printf("z down\n"); break;
		case (SDLK_x): printf("x down\n"); break;
		case (SDLK_c): printf("c down\n"); break;
		case (SDLK_v): printf("v down\n"); break;
		case (SDLK_b): printf("b down\n"); break;
		case (SDLK_n): printf("n down\n"); break;
		case (SDLK_m): printf("m down\n"); break;
		case (SDLK_TAB): printf("tab down\n"); break;
		case (SDLK_CAPSLOCK): printf("caps lock down\n"); break;
		case (SDLK_LSHIFT): printf("left shift down\n"); break;
		case (SDLK_RETURN): printf("enter down\n"); break;
		case (SDLK_SPACE): printf("space down.\n"); break;
		}
	}

	void Application::dispatchKeyupEvent(SDL_Keycode& keyCode) 
	{
		switch (keyCode)
		{
		case (SDLK_ESCAPE): printf("escape down\n"); break;
		case (SDLK_PLUS): printf("plus up\n"); break;
		case (SDLK_0): printf("0 up\n"); break;
		case (SDLK_1): printf("1 up\n"); break;
		case (SDLK_2): printf("2 up\n"); break;
		case (SDLK_3): printf("3 up\n"); break;
		case (SDLK_4): printf("4 up\n"); break;
		case (SDLK_5): printf("5 up\n"); break;
		case (SDLK_6): printf("6 up\n"); break;
		case (SDLK_7): printf("7 up\n"); break;
		case (SDLK_8): printf("8 up\n"); break;
		case (SDLK_9): printf("9 up\n"); break;
		case (SDLK_q): printf("q up\n"); break;
		case (SDLK_w): printf("w up\n"); break;
		case (SDLK_e): printf("e up\n"); break;
		case (SDLK_r): printf("r up\n"); break;
		case (SDLK_t): printf("t up\n"); break;
		case (SDLK_y): printf("y up\n"); break;
		case (SDLK_u): printf("u up\n"); break;
		case (SDLK_i): printf("i up\n"); break;
		case (SDLK_o): printf("o up\n"); break;
		case (SDLK_p): printf("p up\n"); break;
		case (SDLK_a): printf("a up\n"); break;
		case (SDLK_s): printf("s up\n"); break;
		case (SDLK_d): printf("d up\n"); break;
		case (SDLK_f): printf("f up\n"); break;
		case (SDLK_g): printf("g up\n"); break;
		case (SDLK_h): printf("h up\n"); break;
		case (SDLK_j): printf("j up\n"); break;
		case (SDLK_k): printf("k up\n"); break;
		case (SDLK_l): printf("l up\n"); break;
		case (SDLK_z): printf("z up\n"); break;
		case (SDLK_x): printf("x up\n"); break;
		case (SDLK_c): printf("c up\n"); break;
		case (SDLK_v): printf("v up\n"); break;
		case (SDLK_b): printf("b up\n"); break;
		case (SDLK_n): printf("n up\n"); break;
		case (SDLK_m): printf("m up\n"); break;
		case (SDLK_TAB): printf("tab up\n"); break;
		case (SDLK_CAPSLOCK): printf("caps lock up\n"); break;
		case (SDLK_LSHIFT): printf("left shift up\n"); break;
		case (SDLK_RETURN): printf("enter up\n"); break;
		case (SDLK_SPACE): printf("space up.\n"); break;
		}
	}
	void Application::dispatchMouseMoveEvent(SDL_MouseMotionEvent& mouseMotion) 
	{
		printf("%d, %d\n", mouseMotion.x, mouseMotion.y);
	}
	void Application::dispatchMouseButtonDownEvent(SDL_MouseButtonEvent& mouseButtonEvent) 
	{
		switch (mouseButtonEvent.button)
		{
		case (SDL_BUTTON_LEFT): printf("left mouse button clicked.\n"); break;
		case (SDL_BUTTON_MIDDLE): printf("middle mouse button clicked.\n"); break;
		case (SDL_BUTTON_RIGHT): printf("right mouse button clicked.\n"); break;

		default: break;
		}
	}
	void Application::dispatchMouseButtonUpEvent(SDL_MouseButtonEvent& mouseButtonEvent) 
	{
		switch (mouseButtonEvent.button)
		{
		case (SDL_BUTTON_LEFT): printf("left mouse button up.\n"); break;
		case (SDL_BUTTON_MIDDLE): printf("middle mouse button up.\n"); break;
		case (SDL_BUTTON_RIGHT): printf("right mouse button up.\n"); break;

		default: break;
		}
	}
	void Application::dispatchMouseScrollEvent(SDL_MouseWheelEvent& mouseWheelEvent) 
	{
		printf("x: %d, y: %d\n", mouseWheelEvent.x, mouseWheelEvent.y);
		switch (mouseWheelEvent.direction)
		{
		case (SDL_MOUSEWHEEL_NORMAL): printf("mouse wheel normal.\n"); break;
		case (SDL_MOUSEWHEEL_FLIPPED): printf("mouse wheel flipped.\n"); break;
		}
	}
}