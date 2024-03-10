#pragma once

#include "../include/WindowManagerLayer.h"

namespace Engine
{
	WindowManagerLayer::WindowManagerLayer(IEventSystem* eventSystem) : ptrEventSystem(eventSystem), Layer("WindowManagerLayer")
	{
	}

	WindowManagerLayer::~WindowManagerLayer()
	{
	}

	void WindowManagerLayer::onAttach()
	{
	}

	void WindowManagerLayer::onDetach()
	{
	}

	void WindowManagerLayer::onUpdate()
	{
	}

	void WindowManagerLayer::processEvent(Event& e)
	{
		switch (e.eventType)
		{
			case (F_KEY_DOWN):
				ptrEventSystem->triggerEventCallback(ActionType::ToggleFullscreen, std::monostate{});
				ENGINE_TRACE("Handled event: {}", static_cast<int>(F_KEY_DOWN));
				e.handled = true;
				break;
			case (RESIZE_WINDOW):
				ptrEventSystem->triggerEventCallback(ActionType::ResizeWindow, std::make_pair(e.xPos, e.yPos));
				ENGINE_TRACE("Handled event: {}", static_cast<int>(RESIZE_WINDOW));
				e.handled = true;
				break;
			default:
				break;
		}
	}
}