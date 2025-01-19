#pragma once

#include <string>

#include "../../include/EventHandlers/AppManagementEventHandlers.h"


AppManagementEventHandlers::AppManagementEventHandlers(Engine::Entity* cameraEntity, Engine::Entity* playerEntity) 
{
	Engine::IOEventSystem::RegisterIOEventListener(Engine::F_KEY_DOWN, [&](Engine::IOEvent& e)
		{
			// Need interface to toggle fullscreen.
			// refApp.GetWindowRenderer().ToggleFullscreen();
			Engine::Screen::SetFullscreen();
			// Could add a setter for handled state and put the trace in there.
			CLIENT_TRACE_D("Handled event:" + std::to_string(static_cast<int>(Engine::F_KEY_DOWN)));
			e.Handled = true;
		});

	Engine::IOEventSystem::RegisterIOEventListener(Engine::RESIZE_WINDOW, [&](Engine::IOEvent& e)
		{
			// Window Interface to call the resize function.
			// refApp.GetWindowRenderer().ResizeWindow(e.X_POS, e.Y_POS);
			Engine::Screen::SetWindowSize(Engine::Vector2D<int>(e.X_POS, e.Y_POS));
			CLIENT_TRACE_D("Handled event: " + std::to_string(static_cast<int>(Engine::RESIZE_WINDOW)));
			e.Handled = true;
		});

	Engine::IOEventSystem::RegisterIOEventListener(Engine::ESCAPE_KEY_DOWN, [&](Engine::IOEvent& e)
		{
			CLIENT_TRACE_D("Handled event: " + std::to_string(static_cast<int>(Engine::ESCAPE_KEY_DOWN)));
			Engine::EMU::GetInstance()->EndApp();
			e.Handled = true;
		});

	Engine::IOEventSystem::RegisterIOEventListener(Engine::QUIT, [&](Engine::IOEvent& e)
		{
			CLIENT_TRACE_D("Handled event: " + std::to_string(static_cast<int>(Engine::QUIT)));
			Engine::EMU::GetInstance()->EndApp();
			e.Handled = true;
		});

	Engine::IOEventSystem::RegisterIOEventListener(Engine::G_KEY_DOWN, [&](Engine::IOEvent& e)
		{
			CLIENT_TRACE_D("Handled event: " + std::to_string(static_cast<int>(Engine::G_KEY_DOWN)));
			Engine::ECS::GetComponentManager<Engine::Camera>().GetComponent(cameraEntity)->SetPixelsPerUnit(16);
			e.Handled = true;
		});

	Engine::IOEventSystem::RegisterIOEventListener(Engine::H_KEY_DOWN, [&](Engine::IOEvent& e)
		{
			CLIENT_TRACE_D("Handled event: " + std::to_string(static_cast<int>(Engine::H_KEY_DOWN)));
			Engine::ECS::GetComponentManager<Engine::Camera>().GetComponent(cameraEntity)->SetPixelsPerUnit(32);
			e.Handled = true;
		});
	
	Engine::IOEventSystem::RegisterIOEventListener(Engine::L_KEY_DOWN, [&](Engine::IOEvent& e)
		{
			CLIENT_TRACE_D("Handled event: " + std::to_string(static_cast<int>(Engine::L_KEY_DOWN)));
			Engine::EMU::GetInstance()->LoadScene("Level2");
			e.Handled = true;
		});

	Engine::IOEventSystem::RegisterIOEventListener(Engine::O_KEY_DOWN, [&](Engine::IOEvent& e)
		{
			Engine::ECS::Deactivate(playerEntity);
		});

	Engine::IOEventSystem::RegisterIOEventListener(Engine::O_KEY_UP, [&](Engine::IOEvent& e)
		{
			Engine::ECS::Activate(playerEntity);
		});
	


}

AppManagementEventHandlers::~AppManagementEventHandlers() {}