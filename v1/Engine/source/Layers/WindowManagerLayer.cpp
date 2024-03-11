#pragma once

#include "../../include/Layers/WindowManagerLayer.h"
#include "../../include/Events/IEventSystem.h"
#include "../../include/Events/Event.h"
#include "../../include/Logging/Logger.h"

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
		case (Engine::F_KEY_DOWN):
			ptrEventSystem->triggerEventCallback(ActionType::ToggleFullscreen, std::monostate{});
			ENGINE_TRACE("Handled event: {}", static_cast<int>(F_KEY_DOWN));
			e.handled = true;
			break;
		case (Engine::RESIZE_WINDOW):
			ptrEventSystem->triggerEventCallback(ActionType::ResizeWindow, std::make_pair(e.xPos, e.yPos));
			ENGINE_TRACE("Handled event: {}", static_cast<int>(RESIZE_WINDOW));
			e.handled = true;
			break;
		default:
			break;
		}
	}
}


