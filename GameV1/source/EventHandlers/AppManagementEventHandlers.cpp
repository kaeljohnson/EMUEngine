#pragma once

#include <string>

#include "../../include/EventHandlers/AppManagementEventHandlers.h"


AppManagementEventHandlers::AppManagementEventHandlers(Engine::EventManager& eventManager, Engine::Entity* cameraEntity) 
{
	eventManager.RegisterEventHandler(Engine::F_KEY_DOWN, [](Engine::Event& e)
		{
			Engine::ICallbackSystem::GetInstance()->TriggerCallback(Engine::Type::ToggleFullscreen, std::monostate{});
			// Could add a setter for handled state and put the trace in there.
			CLIENT_TRACE_D("Handled event:" + std::to_string(static_cast<int>(Engine::F_KEY_DOWN)));
			e.Handled = true;
		});

	eventManager.RegisterEventHandler(Engine::RESIZE_WINDOW, [](Engine::Event& e)
		{
			Engine::ICallbackSystem::GetInstance()->TriggerCallback(Engine::Type::ResizeWindow, std::make_pair(e.X_POS, e.Y_POS));
			CLIENT_TRACE_D("Handled event: " + std::to_string(static_cast<int>(Engine::RESIZE_WINDOW)));
			e.Handled = true;
		});

	eventManager.RegisterEventHandler(Engine::ESCAPE_KEY_DOWN, [](Engine::Event& e)
		{
			CLIENT_TRACE_D("Handled event: " + std::to_string(static_cast<int>(Engine::ESCAPE_KEY_DOWN)));
			Engine::ICallbackSystem::GetInstance()->TriggerCallback(Engine::Type::EndApplication, std::monostate{});
			e.Handled = true;
		});

	eventManager.RegisterEventHandler(Engine::QUIT, [](Engine::Event& e)
		{
			CLIENT_TRACE_D("Handled event: " + std::to_string(static_cast<int>(Engine::QUIT)));
			Engine::ICallbackSystem::GetInstance()->TriggerCallback(Engine::Type::EndApplication, std::monostate{});
			e.Handled = true;
		});

	eventManager.RegisterEventHandler(Engine::G_KEY_DOWN, [&](Engine::Event& e)
		{
			CLIENT_TRACE_D("Handled event: " + std::to_string(static_cast<int>(Engine::G_KEY_DOWN)));
			Engine::ECS::GetComponentManager<Engine::Camera>().GetComponent(cameraEntity)->SetPixelsPerUnit(16);
			e.Handled = true;
		});

	eventManager.RegisterEventHandler(Engine::H_KEY_DOWN, [&](Engine::Event& e)
		{
			CLIENT_TRACE_D("Handled event: " + std::to_string(static_cast<int>(Engine::H_KEY_DOWN)));
			Engine::ECS::GetComponentManager<Engine::Camera>().GetComponent(cameraEntity)->SetPixelsPerUnit(32);
			e.Handled = true;
		});
	
	eventManager.RegisterEventHandler(Engine::L_KEY_DOWN, [&](Engine::Event& e)
		{
			CLIENT_TRACE_D("Handled event: " + std::to_string(static_cast<int>(Engine::L_KEY_DOWN)));
			Engine::Application::GetInstance()->GetSceneManager().LoadScene("Level2");
			e.Handled = true;
		});
}

AppManagementEventHandlers::~AppManagementEventHandlers() {}