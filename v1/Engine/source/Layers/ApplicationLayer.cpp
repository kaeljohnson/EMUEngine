#pragma once

#include "../../include/Layers/ApplicationLayer.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Events/IEventSystem.h"
#include "../../include/Events/Event.h"

namespace Engine
{
	ApplicationLayer::ApplicationLayer(IEventSystem* eventSystem) : ptrEventSystem(eventSystem), Layer("ApplicationLayer")
	{
	}

	ApplicationLayer::~ApplicationLayer()
	{
	}

	void ApplicationLayer::onAttach()
	{
	}

	void ApplicationLayer::onDetach()
	{
	}

	void ApplicationLayer::onUpdate()
	{
	}

	void ApplicationLayer::processEvent(Event& e)
	{
		switch (e.eventType)
		{
		case (ESCAPE_KEY_DOWN):
			ENGINE_TRACE("Handled event: {}", static_cast<int>(ESCAPE_KEY_DOWN));
			ptrEventSystem->triggerEventCallback(ActionType::EndApplication, std::monostate{});
			e.handled = true;
			break;
		case (QUIT):
			ENGINE_TRACE("Handled event: {}", static_cast<int>(QUIT));
			ptrEventSystem->triggerEventCallback(ActionType::EndApplication, std::monostate{});
			e.handled = true;
			break;
		default:
			break;
		}
	}
}