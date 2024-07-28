#pragma once

#include <string>
#include <Engine.h>

// Example of client defined listener. Processes events related to application management.

class AppManagementEventHandlers
{
private:
	Engine::ICallbackSystemPtr ptrICallbackSystem;

public:
	AppManagementEventHandlers(std::unordered_map<Engine::EventType, Engine::EventHandler>& eventHandlers);
	~AppManagementEventHandlers();
};
