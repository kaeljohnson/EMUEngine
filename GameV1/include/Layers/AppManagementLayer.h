#pragma once

#include <string>

#include <Engine.h>

// Example of client defined layer. Controls the application 
// lifecycle along with some window events.

class AppManagementLayer : public Engine::Layer
{
public:
	AppManagementLayer(std::string name);
	~AppManagementLayer();

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void ProcessEvent(Engine::Event& e) override;
};
