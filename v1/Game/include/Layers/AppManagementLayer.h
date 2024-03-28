#pragma once

#include "../EngineIncludes.h"

// Example of client defined layer. Controls the application 
// lifecycle along with some window events.

class AppManagementLayer : public Engine::Layer
{
public:
	AppManagementLayer(Engine::ICallbackSystem* ptrICallbackSystem);
	~AppManagementLayer();

	void onAttach() override;
	void onDetach() override;
	void onUpdate() override;
	void processEvent(Engine::Event& e) override;
};
