#pragma once

#include <string>
#include <Engine.h>

#include "../ClientObjects/ClientObject.h"

// Example of client defined listener. Processes events related to application management.

class ClientObjectsListener : public Engine::EventListener
{

private:
	ClientObject* ptrClientObject;

public:
	ClientObjectsListener(std::string name, ClientObject* ptrObject);
	~ClientObjectsListener();

	void ProcessEvent(Engine::Event& e) override;
};