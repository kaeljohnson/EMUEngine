#pragma once

#include "../../include/Layers/ApplicationLayer.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Events/IEventAction.h"
#include "../../include/Events/Event.h"

namespace Engine
{
	ApplicationLayer::ApplicationLayer(IEventAction* eventSystem) : ptrEventSystem(eventSystem), Layer("ApplicationLayer")
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
		switch (e.Type)
		{
		case (ESCAPE_KEY_DOWN):
			ENGINE_TRACE("Handled event: {}", static_cast<int>(ESCAPE_KEY_DOWN));
			ptrEventSystem->triggerActionCallback(ActionType::EndApplication, std::monostate{});
			e.Handled = true;
			break;
		case (QUIT):
			ENGINE_TRACE("Handled event: {}", static_cast<int>(QUIT));
			ptrEventSystem->triggerActionCallback(ActionType::EndApplication, std::monostate{});
			e.Handled = true;
			break;
		default:
			break;
		}
	}
}