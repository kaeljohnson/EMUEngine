#pragma once

#include <string>
#include <Engine.h>

// Example of client defined listener. Processes events related to application management.

class AppManagementEventHandlers
{
public:
	AppManagementEventHandlers(Engine::EventManager& eventManager, const size_t cameraEntityID);
	~AppManagementEventHandlers();
};
