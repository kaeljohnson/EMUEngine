#pragma once

#include <string>

#include "../../include/EventHandlers/AppManagementEventHandlers.h"


AppManagementEventHandlers::AppManagementEventHandlers(std::unordered_map<Engine::EventType, Engine::EventHandler>& refEventHandlerMap) 
	: ptrICallbackSystem(Engine::ICallbackSystem::GetInstance()) 
{
	refEventHandlerMap[Engine::F_KEY_DOWN] = [](Engine::Event& e)
		{
			Engine::ICallbackSystem::GetInstance()->TriggerCallback(Engine::Type::ToggleFullscreen, std::monostate{});
			// Could add a setter for handled state and put the trace in there.
			CLIENT_TRACE_D("Handled event:" + std::to_string(static_cast<int>(Engine::F_KEY_DOWN)));
			e.Handled = true;
		};

	refEventHandlerMap[Engine::RESIZE_WINDOW] = [](Engine::Event& e)
		{
			Engine::ICallbackSystem::GetInstance()->TriggerCallback(Engine::Type::ResizeWindow, std::make_pair(e.X_POS, e.Y_POS));
			CLIENT_TRACE_D("Handled event: " + std::to_string(static_cast<int>(Engine::RESIZE_WINDOW)));
			e.Handled = true;
		};

	refEventHandlerMap[Engine::ESCAPE_KEY_DOWN] = [](Engine::Event& e)
		{
			CLIENT_TRACE_D("Handled event: " + std::to_string(static_cast<int>(Engine::ESCAPE_KEY_DOWN)));
			Engine::ICallbackSystem::GetInstance()->TriggerCallback(Engine::Type::EndApplication, std::monostate{});
			e.Handled = true;
		};

	refEventHandlerMap[Engine::QUIT] = [](Engine::Event& e)
		{
			CLIENT_TRACE_D("Handled event: " + std::to_string(static_cast<int>(Engine::QUIT)));
			Engine::ICallbackSystem::GetInstance()->TriggerCallback(Engine::Type::EndApplication, std::monostate{});
			e.Handled = true;
		};
}

AppManagementEventHandlers::~AppManagementEventHandlers() {}