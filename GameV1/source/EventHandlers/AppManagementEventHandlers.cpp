#pragma once

#include <string>

#include "../../include/EventHandlers/AppManagementEventHandlers.h"


AppManagementEventHandlers::AppManagementEventHandlers() 
{
	Engine::EMU::GetInstance()->RegisterIOEventListener(Engine::F_KEY_DOWN, [](Engine::IOEvent& e)
		{
			// Need interface to toggle fullscreen.
			Engine::Screen::SetFullscreen();
			// Could add a setter for handled state and put the trace in there.
			CLIENT_TRACE_D("Handled event:" + std::to_string(static_cast<int>(Engine::F_KEY_DOWN)));
			e.Handled = true;
		});

	Engine::EMU::GetInstance()->RegisterIOEventListener(Engine::RESIZE_WINDOW, [](Engine::IOEvent& e)
		{
			// Window Interface to call the resize function.
			// refApp.GetWindowRenderer().ResizeWindow(e.X_POS, e.Y_POS);
			Engine::Screen::SetWindowSize(Engine::Vector2D<int>(e.X_POS, e.Y_POS));
			CLIENT_TRACE_D("Handled event: " + std::to_string(static_cast<int>(Engine::RESIZE_WINDOW)));
			e.Handled = true;
		});

	Engine::EMU::GetInstance()->RegisterIOEventListener(Engine::ESCAPE_KEY_DOWN, [](Engine::IOEvent& e)
		{
			CLIENT_TRACE_D("Handled event: " + std::to_string(static_cast<int>(Engine::ESCAPE_KEY_DOWN)));
			Engine::EMU::GetInstance()->EndApp();
			e.Handled = true;
		});

	Engine::EMU::GetInstance()->RegisterIOEventListener(Engine::QUIT, [](Engine::IOEvent& e)
		{
			CLIENT_TRACE_D("Handled event: " + std::to_string(static_cast<int>(Engine::QUIT)));
			Engine::EMU::GetInstance()->EndApp();
			e.Handled = true;
		});

	Engine::EMU::GetInstance()->RegisterIOEventListener(Engine::G_KEY_DOWN, [](Engine::IOEvent& e)
		{
			Engine::Entity entity = Engine::EMU::GetInstance()->GetCurrentRuntimeEntity('P');
			CLIENT_TRACE_D("Handled event: " + std::to_string(static_cast<int>(Engine::G_KEY_DOWN)));
			Engine::EMU::GetInstance()->ICAMERA().SetPixelsPerUnit(entity, 16);
			e.Handled = true;
		});

	Engine::EMU::GetInstance()->RegisterIOEventListener(Engine::H_KEY_DOWN, [](Engine::IOEvent& e)
		{
			Engine::Entity entity = Engine::EMU::GetInstance()->GetCurrentRuntimeEntity('P');
			CLIENT_TRACE_D("Handled event: " + std::to_string(static_cast<int>(Engine::H_KEY_DOWN)));
			Engine::EMU::GetInstance()->ICAMERA().SetPixelsPerUnit(entity, 32);
			e.Handled = true;
		});
	
	Engine::EMU::GetInstance()->RegisterIOEventListener(Engine::L_KEY_DOWN, [&](Engine::IOEvent& e)
		{
			CLIENT_TRACE_D("Handled event: " + std::to_string(static_cast<int>(Engine::L_KEY_DOWN)));
			Engine::EMU::GetInstance()->LoadScene("Level2");
			e.Handled = true;
		});

	Engine::EMU::GetInstance()->RegisterIOEventListener(Engine::O_KEY_DOWN, [](Engine::IOEvent& e)
		{
			CLIENT_TRACE_D("Handled event O KEY DOWN: " + std::to_string(static_cast<int>(Engine::O_KEY_DOWN)));
			Engine::Entity testEntity = Engine::EMU::GetInstance()->GetCurrentRuntimeEntity('S');
			Engine::EMU::GetInstance()->Activate(testEntity);
			CLIENT_TRACE_D("Done with O key down listener");
		});

	Engine::EMU::GetInstance()->RegisterIOEventListener(Engine::O_KEY_UP, [](Engine::IOEvent& e)
		{
			CLIENT_TRACE_D("Handled event O KEY UP: " + std::to_string(static_cast<int>(Engine::O_KEY_UP)));
			Engine::Entity testEntity = Engine::EMU::GetInstance()->GetCurrentRuntimeEntity('S');
			Engine::EMU::GetInstance()->Deactivate(testEntity);
			CLIENT_TRACE_D("Done with O key up listener");
		});

	Engine::EMU::GetInstance()->RegisterIOEventListener(Engine::P_KEY_DOWN, [](Engine::IOEvent& e)
		{
			CLIENT_TRACE_D("Handled event P KEY DOWN: " + std::to_string(static_cast<int>(Engine::P_KEY_DOWN)));
			Engine::Entity testEntity = Engine::EMU::GetInstance()->GetCurrentRuntimeEntity('T');
			Engine::EMU::GetInstance()->Activate(testEntity);
			CLIENT_TRACE_D("Done with P key down listener");
		});

	Engine::EMU::GetInstance()->RegisterIOEventListener(Engine::P_KEY_UP, [](Engine::IOEvent& e)
		{
			CLIENT_TRACE_D("Handled event P KEY UP: " + std::to_string(static_cast<int>(Engine::P_KEY_UP)));
			Engine::Entity testEntity = Engine::EMU::GetInstance()->GetCurrentRuntimeEntity('T');
			Engine::EMU::GetInstance()->Deactivate(testEntity);
		});

	/*Engine::EMU::GetInstance()->RegisterIOEventListener(Engine::U_KEY_DOWN, [](Engine::IOEvent& e)
		{
			Engine::Entity testEntity = Engine::EMU::GetInstance()->GetCurrentRuntimeEntity('Y');
			Engine::EMU::GetInstance()->ChangeCamera(testEntity);
		});*/

	/*Engine::EMU::GetInstance()->RegisterIOEventListener(Engine::U_KEY_UP, [](Engine::IOEvent& e)
		{
			Engine::Entity testEntity = Engine::EMU::GetInstance()->GetCurrentRuntimeEntity('P');
			Engine::EMU::GetInstance()->ChangeCamera(testEntity);
		});*/

	Engine::EMU::GetInstance()->RegisterIOEventListener(Engine::T_KEY_DOWN, [](Engine::IOEvent& e)
		{
			Engine::Entity testEntity = Engine::EMU::GetInstance()->GetCurrentRuntimeEntity('P');
			Engine::EMU::GetInstance()->Deactivate(testEntity);
		});
}

AppManagementEventHandlers::~AppManagementEventHandlers() {}