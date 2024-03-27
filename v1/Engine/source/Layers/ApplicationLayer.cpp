#pragma once

#include "../../include/Layers/ApplicationLayer.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Events/Event.h"
#include "../../include/CallbackSystem/CallbackSystem.h"

namespace Engine
{
	ApplicationLayer::ApplicationLayer(ICallbackSystem* ptrICallbackSystem) : Layer("ApplicationLayer", ptrICallbackSystem)
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
			ptrICallbackSystem->triggerCallback(Type::EndApplication, std::monostate{});
			e.Handled = true;
			break;
		case (QUIT):
			ENGINE_TRACE("Handled event: {}", static_cast<int>(QUIT));
			ptrICallbackSystem->triggerCallback(Type::EndApplication, std::monostate{});
			e.Handled = true;
			break;
		default:
			break;
		}
	}
}