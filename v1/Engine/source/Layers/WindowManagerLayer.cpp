#pragma once

#include "../../include/Layers/WindowManagerLayer.h"
#include "../../include/Events/IEventAction.h"
#include "../../include/Events/Event.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	WindowManagerLayer::WindowManagerLayer(IEventAction* eventSystem) : ptrEventSystem(eventSystem), Layer("WindowManagerLayer")
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
		switch (e.Type)
		{
		case (Engine::F_KEY_DOWN):
			ptrEventSystem->triggerActionCallback(ActionType::ToggleFullscreen, std::monostate{});
			ENGINE_TRACE("Handled event: {}", static_cast<int>(F_KEY_DOWN));
			e.Handled = true;
			break;
		case (Engine::RESIZE_WINDOW):
			ptrEventSystem->triggerActionCallback(ActionType::ResizeWindow, std::make_pair(e.X_POS, e.Y_POS));
			ENGINE_TRACE("Handled event: {}", static_cast<int>(RESIZE_WINDOW));
			e.Handled = true;
			break;
		default:
			break;
		}
	}
}


