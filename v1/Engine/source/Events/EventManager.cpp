#pragma once

#include <queue>

#include "../../include/Events/EventManager.h"
#include "../../include/Events/EventHandlers.h"
#include "../../include/Actions/ActionsEnum.h"

namespace Engine
{
	EventManager::EventManager(EventHandlers& eventHandlers, std::queue<Actions>& actionsQ) 
		:  refEventHandlers(eventHandlers), refActionsQ(actionsQ) {}

	void EventManager::handleEvents(SDL_Event& e)
	{

		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case (SDL_QUIT): refEventHandlers.quit(refActionsQ); break;
			case (SDL_WINDOWEVENT): dispatchWindowEvent(e.window); break;
			case (SDL_KEYDOWN): dispatchKeydownEvent(e.key.keysym.sym); break;
			case (SDL_KEYUP): dispatchKeyupEvent(e.key.keysym.sym); break;
			case (SDL_MOUSEBUTTONDOWN): dispatchMouseButtonDownEvent(e.button); break;
			case (SDL_MOUSEBUTTONUP): dispatchMouseButtonUpEvent(e.button); break;
			case (SDL_MOUSEMOTION): dispatchMouseMoveEvent(e.motion); break;
			case (SDL_MOUSEWHEEL): dispatchMouseScrollEvent(e.wheel); break;
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
	}

	void EventManager::dispatchWindowEvent(SDL_WindowEvent& windowEvent)
	{
		switch (windowEvent.type)
		{
		// case (SDL_WINDOWEVENT_RESIZED): m_windowManager.resize(windowEvent.data1, windowEvent.data2); break;
		case (SDL_WINDOWEVENT_RESIZED): refEventHandlers.windowResize(refActionsQ);
		}
	}

	void EventManager::dispatchKeydownEvent(SDL_Keycode& keyCode)
	{
		printf("dispatch keydown event.\n");
		switch (keyCode)
		{
		case (SDLK_ESCAPE): refEventHandlers.escapeKeyDown(); break;
		case (SDLK_EQUALS): refEventHandlers.equalsDown(refActionsQ); break;
		case (SDLK_0): refEventHandlers.zeroKeyDown(); break;
		case (SDLK_1): refEventHandlers.oneKeyDown(); break;
		case (SDLK_2): refEventHandlers.twoKeyDown(); break;
		case (SDLK_3): refEventHandlers.threeKeyDown(); break;
		case (SDLK_4): refEventHandlers.fourKeyDown(); break;
		case (SDLK_5): refEventHandlers.fiveKeyDown(); break;
		case (SDLK_6): refEventHandlers.sixKeyDown(); break;
		case (SDLK_7): refEventHandlers.sevenKeyDown(); break;
		case (SDLK_8): refEventHandlers.eightKeyDown(); break;
		case (SDLK_9): refEventHandlers.nineKeyDown(); break;
		case (SDLK_q): refEventHandlers.qKeyDown(); break;
		case (SDLK_w): refEventHandlers.wKeyDown(); break;
		case (SDLK_e): refEventHandlers.eKeyDown(); break;
		case (SDLK_r): refEventHandlers.rKeyDown(); break;
		case (SDLK_t): refEventHandlers.tKeyDown(); break;
		case (SDLK_y): refEventHandlers.yKeyDown(); break;
		case (SDLK_u): refEventHandlers.uKeyDown(); break;
		case (SDLK_i): refEventHandlers.iKeyDown(); break;
		case (SDLK_o): refEventHandlers.oKeyDown(); break;
		case (SDLK_p): refEventHandlers.pKeyDown(); break;
		case (SDLK_a): refEventHandlers.aKeyDown(); break;
		case (SDLK_s): refEventHandlers.sKeyDown(); break;
		case (SDLK_d): refEventHandlers.dKeyDown(); break;
		case (SDLK_f): refEventHandlers.fKeyDown(); break;
		case (SDLK_g): refEventHandlers.gKeyDown(); break;
		case (SDLK_h): refEventHandlers.hKeyDown(); break;
		case (SDLK_j): refEventHandlers.jKeyDown(); break;
		case (SDLK_k): refEventHandlers.kKeyDown(); break;
		case (SDLK_l): refEventHandlers.lKeyDown(); break;
		case (SDLK_z): refEventHandlers.zKeyDown(); break;
		case (SDLK_x): refEventHandlers.xKeyDown(); break;
		case (SDLK_c): refEventHandlers.cKeyDown(); break;
		case (SDLK_v): refEventHandlers.vKeyDown(); break;
		case (SDLK_b): refEventHandlers.bKeyDown(); break;
		case (SDLK_n): refEventHandlers.nKeyDown(); break;
		case (SDLK_m): refEventHandlers.mKeyDown(); break;
		case (SDLK_TAB): refEventHandlers.tabKeyDown(); break;
		case (SDLK_CAPSLOCK): refEventHandlers.capslockKeyDown(); break;
		case (SDLK_LSHIFT): refEventHandlers.lShiftKeyDown(); break;
		case (SDLK_RETURN): refEventHandlers.enterKeyDown(); break;
		case (SDLK_LEFT): refEventHandlers.leftArrowKeyDown(); break;
		case (SDLK_UP): refEventHandlers.upArrowKeyDown(); break;
		case (SDLK_RIGHT):refEventHandlers.rightArrowKeyDown(); break;
		case (SDLK_DOWN): refEventHandlers.downArrowKeyDown(); break;
		case (SDLK_SPACE): refEventHandlers.spaceKeyDown(); break;
		}
	}

	void EventManager::dispatchKeyupEvent(SDL_Keycode& keyCode)
	{
		switch (keyCode)
		{
		case (SDLK_ESCAPE): refEventHandlers.escapeKeyUp(); break;
		case (SDLK_EQUALS): refEventHandlers.equalsUp(); break;
		case (SDLK_0): refEventHandlers.zeroKeyUp(); break;
		case (SDLK_1): refEventHandlers.oneKeyUp(); break;
		case (SDLK_2): refEventHandlers.twoKeyUp(); break;
		case (SDLK_3): refEventHandlers.threeKeyUp(); break;
		case (SDLK_4): refEventHandlers.fourKeyUp(); break;
		case (SDLK_5): refEventHandlers.fiveKeyUp(); break;
		case (SDLK_6): refEventHandlers.sixKeyUp(); break;
		case (SDLK_7): refEventHandlers.sevenKeyUp(); break;
		case (SDLK_8): refEventHandlers.eightKeyUp(); break;
		case (SDLK_9): refEventHandlers.nineKeyUp(); break;
		case (SDLK_q): refEventHandlers.qKeyUp(); break;
		case (SDLK_w): refEventHandlers.wKeyUp(); break;
		case (SDLK_e): refEventHandlers.eKeyUp(); break;
		case (SDLK_r): refEventHandlers.rKeyUp(); break;
		case (SDLK_t): refEventHandlers.tKeyUp(); break;
		case (SDLK_y): refEventHandlers.yKeyUp(); break;
		case (SDLK_u): refEventHandlers.uKeyUp(); break;
		case (SDLK_i): refEventHandlers.iKeyUp(); break;
		case (SDLK_o): refEventHandlers.oKeyUp(); break;
		case (SDLK_p): refEventHandlers.pKeyUp(); break;
		case (SDLK_a): refEventHandlers.aKeyUp(); break;
		case (SDLK_s): refEventHandlers.sKeyUp(); break;
		case (SDLK_d): refEventHandlers.dKeyUp(); break;
		case (SDLK_f): refEventHandlers.fKeyUp(); break;
		case (SDLK_g): refEventHandlers.gKeyUp(); break;
		case (SDLK_h): refEventHandlers.hKeyUp(); break;
		case (SDLK_j): refEventHandlers.jKeyUp(); break;
		case (SDLK_k): refEventHandlers.kKeyUp(); break;
		case (SDLK_l): refEventHandlers.lKeyUp(); break;
		case (SDLK_z): refEventHandlers.zKeyUp(); break;
		case (SDLK_x): refEventHandlers.xKeyUp(); break;
		case (SDLK_c): refEventHandlers.cKeyUp(); break;
		case (SDLK_v): refEventHandlers.vKeyUp(); break;
		case (SDLK_b): refEventHandlers.bKeyUp(); break;
		case (SDLK_n): refEventHandlers.nKeyUp(); break;
		case (SDLK_m): refEventHandlers.mKeyUp(); break;
		case (SDLK_TAB): refEventHandlers.tabKeyUp(); break;
		case (SDLK_CAPSLOCK): refEventHandlers.capslockKeyUp(); break;
		case (SDLK_LSHIFT): refEventHandlers.lShiftKeyUp(); break;
		case (SDLK_RETURN): refEventHandlers.enterKeyUp(); break;
		case (SDLK_LEFT): refEventHandlers.leftArrowKeyUp(); break;
		case (SDLK_UP): refEventHandlers.upArrowKeyUp(); break;
		case (SDLK_RIGHT): refEventHandlers.rightArrowKeyUp(); break;
		case (SDLK_DOWN): refEventHandlers.downArrowKeyUp(); break;
		case (SDLK_SPACE): refEventHandlers.spaceKeyUp(); break;
		}
	}
	void EventManager::dispatchMouseMoveEvent(SDL_MouseMotionEvent& mouseMotion)
	{
		refEventHandlers.mouseMove(mouseMotion.x, mouseMotion.y);
	}
	void EventManager::dispatchMouseButtonDownEvent(SDL_MouseButtonEvent& mouseButtonEvent)
	{
		switch (mouseButtonEvent.button)
		{
		case (SDL_BUTTON_LEFT): refEventHandlers.leftMouseButtonDown(); break;
		case (SDL_BUTTON_MIDDLE): refEventHandlers.middleMouseButtonDown(); break;
		case (SDL_BUTTON_RIGHT): refEventHandlers.rightMouseButtonDown(); break;

		default: break;
		}
	}
	void EventManager::dispatchMouseButtonUpEvent(SDL_MouseButtonEvent& mouseButtonEvent)
	{
		switch (mouseButtonEvent.button)
		{
		case (SDL_BUTTON_LEFT): refEventHandlers.leftMouseButtonUp(); break;
		case (SDL_BUTTON_MIDDLE): refEventHandlers.middleMouseButtonUp(); break;
		case (SDL_BUTTON_RIGHT): refEventHandlers.rightMouseButtonUp(); break;

		default: break;
		}
	}
	void EventManager::dispatchMouseScrollEvent(SDL_MouseWheelEvent& mouseWheelEvent)
	{
		switch (mouseWheelEvent.direction)
		{
		case (SDL_MOUSEWHEEL_NORMAL): refEventHandlers.mouseScroll(mouseWheelEvent.x, mouseWheelEvent.y); break;
		}
	}

	EventHandlers& EventManager::getEventHandlers()
	{
		return refEventHandlers;
	}

}