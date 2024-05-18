#include <string>

#include "../../include/EventListeners/ClientObjectsListener.h"


ClientObjectsListener::ClientObjectsListener(std::string name, ClientObject* ptrObject) : ptrClientObject(ptrObject), Engine::EventListener()
{
}

ClientObjectsListener::~ClientObjectsListener()
{
}

void ClientObjectsListener::ProcessEvent(Engine::Event& e)
{
	e.Handled = true;
	if (e.Type == Engine::D_KEY_DOWN)
	{
		ptrClientObject->SetXVelocity(50.0f);
	}
	else if (e.Type == Engine::A_KEY_DOWN)
	{
		ptrClientObject->SetXVelocity(-50.0f);
	}
	else if (e.Type == Engine::W_KEY_DOWN)
	{
		ptrClientObject->SetYVelocity(-50.0f);
	}
	else if (e.Type == Engine::S_KEY_DOWN)
	{
		ptrClientObject->SetYVelocity(50.0f);
	}
	else if (e.Type == Engine::D_KEY_UP || e.Type == Engine::A_KEY_UP)
	{
		ptrClientObject->SetXVelocity(0.0f);
	}
	else if (e.Type == Engine::W_KEY_UP || e.Type == Engine::S_KEY_UP)
	{
		ptrClientObject->SetYVelocity(0.0f);
	}
	else
	{
		e.Handled = false;
	}
}