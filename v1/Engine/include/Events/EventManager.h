#pragma once

#include <SDL.h>
#include <SDL_image.h>

namespace Engine
{
	struct EVENT_MAP
	{
		bool ESC = 0;

		bool ONE = 0;
		bool TWO = 0;
		bool THREE = 0;
		bool FOUR = 0;

		bool A_PRESS = 0;
		bool A_HOLD = 0;
		bool A_RELEASE = 0;
		bool W_PRESS = 0;
		bool W_HOLD = 0;
		bool W_RELEASE = 0;
		bool S_PRESS = 0;
		bool S_HOLD = 0;
		bool S_RELEASE = 0;
		bool D_PRESS = 0;
		bool D_HOLD = 0;
		bool D_RELEASE = 0;

		bool SPACE_PRESS = 0;
		bool SPACE_HOLD = 0;
		bool SPACE_RELEASE = 0;

		bool Z = 0;
		bool X = 0;
		bool C = 0;

		bool F = 0;
		bool G = 0;
		bool R = 0; 

		bool LEFT_MOUSE_CLICK = 0;
		bool LEFT_MOUSE_HOLD = 0;
		bool LEFT_MOUSE_RELEASE = 0;
		bool RIGHT_MOUSE_CLICK = 0;
		bool RIGHT_MOUSE_HOLD = 0;
		bool RIGHT_MOUSE_RELEASE = 0;
	};

	/* enum EVENT_TYPE
	{
		None = 0,
		
		WindowClose,
		WindowAndCameraResize,
		WindowMove,

		KeyPress,
		KeyHold,
		KeyRelease,

		MouseButtonPress,
		MouseButtonRelease,
		MouseMove,
		MouseScroll
	}; */

	class EventManager
	{
	private:

	public:
		void propogateEventQueue(SDL_Event& e);
		bool pollEvents(SDL_Event& e);


	};
}