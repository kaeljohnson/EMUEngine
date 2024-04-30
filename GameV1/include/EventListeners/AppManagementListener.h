#pragma once

#include <string>

#include <Engine.h>

// Example of client defined listener. Processes events related to application management.

class AppManagementListener : public Engine::EventListener
{
public:
	AppManagementListener(std::string name);
	~AppManagementListener();

	void ProcessEvent(Engine::Event& e) override;
};
